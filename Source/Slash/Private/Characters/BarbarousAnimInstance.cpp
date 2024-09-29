#include <Characters/BarbarousAnimInstance.h>
#include <Characters/BarbarousPlayer.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>

void UBarbarousAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();

	BarbarousPlayer = Cast<ABarbarousPlayer>(TryGetPawnOwner());
	
	if(BarbarousPlayer == nullptr) { return; }

	BarbarousCharacterMovement = BarbarousPlayer->GetCharacterMovement();
}

void UBarbarousAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(BarbarousPlayer == nullptr) {
		return;
	}

	// Set ground and isFalling variables with correct values
	GroundSpeed = UKismetMathLibrary::VSizeXY(BarbarousCharacterMovement->Velocity);
}
