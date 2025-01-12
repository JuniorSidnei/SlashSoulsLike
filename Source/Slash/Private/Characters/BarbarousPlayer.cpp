#include <Characters/BarbarousPlayer.h>
#include <Components/InputComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/AttributeComponent.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Weapon/Weapon.h>
#include <Animation/AnimMontage.h>
#include <HUD/GameHUD.h>
#include <HUD/HUDOverlay.h>
#include <GameFramework/PlayerState.h>
#include <Items/Collectable.h>
#include <Items/Soul.h>

ABarbarousPlayer::ABarbarousPlayer() {
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.0f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ABarbarousPlayer::BeginPlay() {
	Super::BeginPlay();

	Tags.Add(FName("Player"));
	
	// Add barbarous mapping context to EnhancedInputSystem
	auto playerController = Cast<APlayerController>(Controller);
	
	if(!playerController) { return;	}

	if(UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer())) {
		subSystem->AddMappingContext(BarbarousInputMappingContext, 0);
	}

	auto gameHUD = Cast<AGameHUD>(playerController->GetHUD());
	
	if(!gameHUD) { return; }

	m_hudOverlay = gameHUD->GetHUDOverlay();

	if(!m_hudOverlay) { return; }
	
	m_hudOverlay->Init();
	
}

void ABarbarousPlayer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	m_hudOverlay->SetStaminaBarPercent(AttributeComponent->GetStaminaPercent());
}

void ABarbarousPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	auto enhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	if(enhancedInputComponent == nullptr) {
		return;
	}  

	// Bind actions input
	enhancedInputComponent->BindAction(MovementActionInput, ETriggerEvent::Triggered, this, &ABarbarousPlayer::Move);
	enhancedInputComponent->BindAction(CameraLookActionInput, ETriggerEvent::Triggered, this, &ABarbarousPlayer::CameraLook);
	enhancedInputComponent->BindAction(DodgeActionInput, ETriggerEvent::Triggered, this, &ABarbarousPlayer::Dodge);
	enhancedInputComponent->BindAction(EquipActionInput, ETriggerEvent::Triggered, this, &ABarbarousPlayer::EquipWeapon);
	enhancedInputComponent->BindAction(AttackActionInput, ETriggerEvent::Triggered, this, &ABarbarousPlayer::Attack);
}

void ABarbarousPlayer::SetOverlappingItem(AItems* item) {
	m_currentOverlappingItem = item;
}

void ABarbarousPlayer::AddSouls(ASoul* soul) {
	AttributeComponent->AddSouls(soul->GetRandomSoulAmount());
	m_hudOverlay->SetSouls(AttributeComponent->GetSoulsAmount());
}

void ABarbarousPlayer::AddGold(ACollectable* collectable) {
	AttributeComponent->AddGold(collectable->GetRandomSoulAmount());
	m_hudOverlay->SetGold(AttributeComponent->GetGoldAmount());
}

void ABarbarousPlayer::HitReactEnd() {
	CurrentActionState = EAction::Unoccupied;
}

void ABarbarousPlayer::DeathReactEnd() {
	CurrentActionState = EAction::Death;
}

void ABarbarousPlayer::Move(const FInputActionValue& value) {
	if(CurrentActionState != EAction::Unoccupied) {
		return;
	}
	
	const FVector2d movementVector = value.Get<FVector2d>();

    // Get controller rotation and create yaw rotation 
	const FRotator rotation = Controller->GetControlRotation();
	const FRotator yawRotation(0.f, rotation.Yaw, 0.f);
	
	// Get actor forward based in the rotation direction and apply movement with Y value
	const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(forwardDirection, movementVector.Y);

	// Get actor right based in the rotation direction and apply movement with X
	const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(rightDirection, movementVector.X);
}

void ABarbarousPlayer::CameraLook(const FInputActionValue& value) {
	const FVector2d lookAxisVector = value.Get<FVector2d>();

	AddControllerPitchInput(lookAxisVector.Y);
	AddControllerYawInput(lookAxisVector.X);
} 

void ABarbarousPlayer::Dodge() {
	if(CurrentActionState != EAction::Unoccupied) {
		return;
	}

	if(!AttributeComponent->HasDodgeStamina()) {
		return;
	}

	
	CurrentActionState = EAction::Dodging;
	PlayMontage(DodgeMontage);
	AttributeComponent->TakeDodgeStamina();
}

void ABarbarousPlayer::EquipWeapon() {
	AWeapon* overlappingWeapon = Cast<AWeapon>(m_currentOverlappingItem);

	if(!overlappingWeapon) return;

	overlappingWeapon->Equip(GetMesh(), FName("hand_rSocket"), this, this);
	m_currentState = ECharacterState::EquippedOneHandWeapon;
	m_currentOverlappingItem = nullptr;
	m_currentWeapon = overlappingWeapon;
}

void ABarbarousPlayer::Attack() {
	if(CurrentActionState != EAction::Unoccupied || m_currentState == ECharacterState::Unequipped) {
		return;
	}

	if(AttributeComponent->GetStamina() < m_currentWeapon->StaminaAttackCost) {
		return;
	}
	
	CurrentActionState = EAction::Attacking;

	PlayMontage(AttackMontage);

	FString sectionString = FString::Printf(TEXT("Combo%d"), m_comboIndex);
	FName sectionName = FName(*sectionString);
	
	auto animInstance = GetMesh()->GetAnimInstance();
	animInstance->Montage_JumpToSection(sectionName);
	
	AttributeComponent->TakeStamina(m_currentWeapon->StaminaAttackCost);
}

void ABarbarousPlayer::Die() {
	GetPlayerState()->DisableInput(Cast<APlayerController>(Controller));
}

void ABarbarousPlayer::ComboEnd() {
	CurrentActionState = EAction::Unoccupied;
	
	m_comboIndex += 1;
	
	if(m_comboIndex > m_maxCombo) {
		m_comboIndex = 1;
	}
}

void ABarbarousPlayer::Hit_Implementation(const FVector& impactPoint, AActor* otherActor) {
	if(CurrentActionState == EAction::Dodging) { return; }
	
	Super::Hit_Implementation(impactPoint, otherActor);

	StopAttackMontage();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	if(AttributeComponent->IsAlive()) {
		CurrentActionState = EAction::HitReaction;
	}
}

float ABarbarousPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	if(AttributeComponent == nullptr || CurrentActionState == EAction::Dodging) { return 0.0f; }
	
	AttributeComponent->TakeDamage(DamageAmount);

	m_hudOverlay->SetHealthBarPercent(AttributeComponent->GetHealthPercent());
	
	return DamageAmount;
}
