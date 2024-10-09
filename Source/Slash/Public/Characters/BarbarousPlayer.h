#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>
#include <InputActionValue.h>
#include <Enums/CharacterStates.h>
#include <BarbarousPlayer.generated.h>

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItems;
class UAnimMontage;

UCLASS()
class SLASH_API ABarbarousPlayer : public ACharacter {
	GENERATED_BODY()

public:
	ABarbarousPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Dodge getter/setter
	FORCEINLINE bool GetDodge() const { return m_isDodging; }
	FORCEINLINE void SetIsDodging(bool value) {m_isDodging = value; }

	// Character state getter/setter
	FORCEINLINE ECharacterState GetCurrentState() const { return m_currentState; }
	FORCEINLINE void SetNewState(ECharacterState newState) { m_currentState = newState; }

	// Set ovelapping item
	FORCEINLINE void SetOverlappingItem(AItems* item) { m_currentOverlappingItem = item; }

	// Camera boom component
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	// View camera component
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	// Player attack montage
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	// Current action state of the player
	UPROPERTY(BlueprintReadWrite, Category = ActionState)
	EAction CurrentActionState = EAction::Unoccupied;
	
protected:
	virtual void BeginPlay() override;

	// Player input actions
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* BarbarousInputMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementActionInput;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* CameraLookActionInput;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeActionInput;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EquipActionInput;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackActionInput;

	// Player actions
	void Move(const FInputActionValue& value);
	void CameraLook(const FInputActionValue& value);
	void Dodge();
	void EquipWeapon();
	void Attack();

	UFUNCTION(BlueprintCallable)
	void ComboEnd();
	
private:
	UPROPERTY()
	AItems* m_currentOverlappingItem;

	ECharacterState m_currentState = ECharacterState::Unequipped;

	uint8_t m_comboIndex = 1;
	bool m_isDodging;
};
