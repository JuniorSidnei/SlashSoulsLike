#pragma once

#include <CoreMinimal.h>
#include <Animation/AnimInstance.h>
#include <Enums/CharacterStates.h>
#include <BarbarousAnimInstance.generated.h>

UCLASS()
class SLASH_API UBarbarousAnimInstance : public UAnimInstance {
	GENERATED_BODY()

public:
	// Initialize and update
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	// Ref to player
	UPROPERTY(BlueprintReadOnly)
	class ABarbarousPlayer* BarbarousPlayer;

	// Ref to character movement component
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* BarbarousCharacterMovement;

	// Ground speed
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	// Dodge flag
	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool IsDodging;

	// State value
	UPROPERTY(BlueprintReadOnly, Category = CharacterState)
	ECharacterState CharacterState;
};