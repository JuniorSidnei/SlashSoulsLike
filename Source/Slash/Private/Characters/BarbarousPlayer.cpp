#include <Characters/BarbarousPlayer.h>
#include <Components/InputComponent.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Weapon/Weapon.h>

ABarbarousPlayer::ABarbarousPlayer() {
	PrimaryActorTick.bCanEverTick = true;

	// Create camera boom component 
	m_cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	// Attach to root component
	m_cameraBoom->SetupAttachment(GetRootComponent());
	m_cameraBoom->TargetArmLength = 300.0f;

	// Create view camera component
	m_viewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	// Attach to camera boom
	m_viewCamera->SetupAttachment(m_cameraBoom);

	// Set values for rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
}

void ABarbarousPlayer::BeginPlay() {
	Super::BeginPlay();

	// Add barbarous mapping context to EnhancedInputSystem
	if(APlayerController* playerController = Cast<APlayerController>(Controller)) {
		if(UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer())) {
			subSystem->AddMappingContext(BarbarousInputMappingContext, 0);
		}
	}
}


void ABarbarousPlayer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ABarbarousPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Cast the playerInput to enhancedInput
	auto enhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	if(enhancedInputComponent == nullptr) {
		return;
	}  

	// Bind actions input
	enhancedInputComponent->BindAction(MovementActionInput, ETriggerEvent::Triggered, this, &ABarbarousPlayer::Move);
	enhancedInputComponent->BindAction(CameraLookActionInput, ETriggerEvent::Triggered, this, &ABarbarousPlayer::CameraLook);
	enhancedInputComponent->BindAction(DodgeActionInput, ETriggerEvent::Triggered, this, &ABarbarousPlayer::Dodge);
	enhancedInputComponent->BindAction(EquipActionInput, ETriggerEvent::Triggered, this, &ABarbarousPlayer::EquipWeapon);
}

void ABarbarousPlayer::Move(const FInputActionValue& value) {
	// Movement vector from input action
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
	// Camera movement vector from input action
	const FVector2d lookAxisVector = value.Get<FVector2d>();

	// Set Pitch and Yaw from mouse movement
	AddControllerPitchInput(lookAxisVector.Y);
	AddControllerYawInput(lookAxisVector.X);
}

void ABarbarousPlayer::Dodge() {
	if(m_isDodging) return;

	m_isDodging = true;
}

void ABarbarousPlayer::EquipWeapon() {

	// Cast the current overlapping item to Weapon class
	auto overlappingWeapon = Cast<AWeapon>(m_currentOverlappingItem);

	// Validate cast
	if(overlappingWeapon == nullptr) return;

	// Equip the weapon
	overlappingWeapon->Equip(GetMesh(), FName("hand_rSocket"));
	m_currentState = ECharacterState::EquippedOneHandWeapon;
}
