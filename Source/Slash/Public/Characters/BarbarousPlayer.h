#pragma once

#include <CoreMinimal.h>
#include <InputActionValue.h>
#include <Enums/CharacterStates.h>
#include <Characters/BaseCharacter.h>
#include <Interfaces/Pickable.h>
#include <BarbarousPlayer.generated.h>

class UHUDOverlay;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItems;
class ASoul;
class UAnimMontage;
class ACollectable;

UCLASS()
class SLASH_API ABarbarousPlayer : public ABaseCharacter, public IPickable {
	GENERATED_BODY()

public:
	ABarbarousPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SetOverlappingItem(AItems* item) override;
	virtual void AddSouls(ASoul* soul) override;
	virtual void AddGold(ACollectable* collectable) override;
	
	FORCEINLINE ECharacterState GetCurrentState() const { return m_currentState; }
	FORCEINLINE void SetNewState(ECharacterState newState) { m_currentState = newState; }

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	UFUNCTION(BlueprintCallable)
	void DeathReactEnd();
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
	
	UPROPERTY(BlueprintReadWrite, Category = ActionState)
	EAction CurrentActionState = EAction::Unoccupied;

protected:
	
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

	virtual void BeginPlay() override;
	virtual void Attack() override;
	virtual void Die() override;
	virtual void ComboEnd() override;
	virtual void Hit_Implementation(const FVector& impactPoint, AActor* otherActor) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void Move(const FInputActionValue& value);
	void CameraLook(const FInputActionValue& value);
	void Dodge();
	void EquipWeapon();
	
private:
	UPROPERTY()
	AItems* m_currentOverlappingItem;

	ECharacterState m_currentState = ECharacterState::Unequipped;

	UPROPERTY()
	UHUDOverlay* m_hudOverlay;
	
	uint8_t m_comboIndex = 1;
	uint8_t m_maxCombo = 4;
	bool m_isDodging;
};
