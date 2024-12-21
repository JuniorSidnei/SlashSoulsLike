#include <Characters/BarbarousPlayer.h>
#include <Components/InputComponent.h>
#include <Components/StaticMeshComponent.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Weapon/Weapon.h>
#include <Animation/AnimMontage.h>

ABarbarousPlayer::ABarbarousPlayer() {
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.0f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	// Set values for rotation
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

	// Add barbarous mapping context to EnhancedInputSystem
	if(APlayerController* playerController = Cast<APlayerController>(Controller)) {
		if(UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer())) {
			subSystem->AddMappingContext(BarbarousInputMappingContext, 0);
		}
	}

	Tags.Add(FName("Player"));
}

void ABarbarousPlayer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
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

void ABarbarousPlayer::HitReactEnd() {
	CurrentActionState = EAction::Unoccupied;
}

void ABarbarousPlayer::Move(const FInputActionValue& value) {
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
	if(m_isDodging) return;

	m_isDodging = true;
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
	
	CurrentActionState = EAction::Attacking;
	
	auto animInstance = GetMesh()->GetAnimInstance();

	if(!animInstance && !AttackMontage) return;

	animInstance->Montage_Play(AttackMontage);

	FString sectionString = FString::Printf(TEXT("Combo%d"), m_comboIndex);
	FName sectionName = FName(*sectionString);

	animInstance->Montage_JumpToSection(sectionName);
}

void ABarbarousPlayer::Die() {
	PlayMontage(DeathMontage);
}

void ABarbarousPlayer::ComboEnd() {
	CurrentActionState = EAction::Unoccupied;
	
	m_comboIndex += 1;
	
	if(m_comboIndex > m_maxCombo) {
		m_comboIndex = 1;
	}
}

void ABarbarousPlayer::Hit_Implementation(const FVector& impactPoint, AActor* otherActor) {
	Super::Hit_Implementation(impactPoint, otherActor);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	CurrentActionState = EAction::HitReaction;
}
