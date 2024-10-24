#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>
#include <Interfaces/IHitable.h>
#include <Enemy.generated.h>

class UAnimMontage;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHitable {
	GENERATED_BODY()

public:
	AEnemy();
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;
	
protected:
	virtual void BeginPlay() override;
	virtual void Hit(const FVector& impactPoint) override;
	
	void PlayHitReactMontage();
};
