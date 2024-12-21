#pragma once

#include <CoreMinimal.h>
#include <Interfaces/IHitable.h>
#include <GameFramework/Character.h>
#include <BaseCharacter.generated.h>	

class AWeapon;
class UAttributeComponent;

UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitable {
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
	AWeapon* m_currentWeapon;
	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, Category = Attributes)
	UAttributeComponent* AttributeComponent;
	
	virtual void BeginPlay() override;

	virtual void Attack();
	
	UFUNCTION(BlueprintCallable)
	virtual void ComboEnd();
	virtual void Die();
	virtual void PlayMontage(UAnimMontage* montage) const;

	virtual void Hit_Implementation(const FVector& impactPoint, AActor* otherActor) override;
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type enabled);

};
