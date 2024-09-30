#include <Characters/BarbarousPlayer.h>
#include <Components/InputComponent.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

ABarbarousPlayer::ABarbarousPlayer() {
	PrimaryActorTick.bCanEverTick = true;
	
	m_cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_cameraBoom->SetupAttachment(GetRootComponent());
	m_cameraBoom->TargetArmLength = 300.0f;

	m_viewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	m_viewCamera->SetupAttachment(m_cameraBoom);

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

	// Bind actions input
	UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	if(enhancedInputComponent == nullptr) {
		return;
	}  

	enhancedInputComponent->BindAction(MovementActionInput, ETriggerEvent::Triggered, this, &ABarbarousPlayer::Move);
	enhancedInputComponent->BindAction(CameraLookActionInput, ETriggerEvent::Triggered, this, &ABarbarousPlayer::CameraLook);
	enhancedInputComponent->BindAction(DodgeActionInput, ETriggerEvent::Triggered, this, &ABarbarousPlayer::Dodge);
}

bool ABarbarousPlayer::GetDodge() const {
	return m_isDodging;
}

void ABarbarousPlayer::SetIsDodging(bool value) {
	m_isDodging = value;
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
	//LaunchCharacter(GetActorForwardVector() * 1000, true, true);
}