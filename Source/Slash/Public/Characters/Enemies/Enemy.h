#pragma once

#include <CoreMinimal.h>
#include <AIController.h>
#include <Perception/PawnSensingComponent.h>
#include <Characters/BaseCharacter.h>
#include <Enemy.generated.h>

class UAnimMontage;
class UHealthBarWidgetComponent;
class AAIController;
class UPawnSensingComponent;
class AWeapon;

UENUM(BlueprintType)
enum class EEnemyDeathPose : uint8 {
	DeadPoseOne,
	DeadPoseTwo
};

UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	Idle,
	Patrolling,
	Waiting,
	ChasingTarget,
	Attacking,
	Engaged,
	Dead
};

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter {
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = HUD)
	UHealthBarWidgetComponent* HealthBarComponent;

	UPROPERTY(BlueprintReadOnly, Category = Montages)
	EEnemyDeathPose EnemyDeathPose;

	UPROPERTY()
	AActor* Target;

	UPROPERTY(BlueprintReadOnly, Category = AI)
	EEnemyState State = EEnemyState::Patrolling;
	
	UPROPERTY(EditAnywhere, Category = AI)
	double SightDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Category = AI)
	double PatrolSightDistance = 200.0f;
	
	UPROPERTY(EditInstanceOnly, Category = AI)
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = AI)
	TArray<AActor*> PatrolPoints;
	
	FTimerHandle PatrolDelayTimer;

	UPROPERTY(EditAnywhere, Category = AI)
	float MinPatrolDelayTime = 2.f;
	
	UPROPERTY(EditAnywhere, Category = AI)
	float MaxPatrolDelayTime = 4.f;

	UPROPERTY()
	AAIController* EnemyController;

	UPROPERTY(VisibleAnywhere, Category = AI)
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditAnywhere, Category = AI)
	float PeripheralVisionAngle = 60.f;

	UPROPERTY(EditAnywhere, Category = AI)
	float SightRadius = 4000.f;

	UPROPERTY(EditAnywhere, Category = AI)
	float AttackDistance = 250.f;
	
	UPROPERTY(EditAnywhere, Category = Character)
	float WalkSpeed = 150.f;
	
	UPROPERTY(EditAnywhere, Category = Character)
	float RunSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<AWeapon> Weapon;

	FTimerHandle AttackDelayTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float MinAttackDelayTime = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float MaxAttackDelayTime = 1.f;
	
	AEnemy();
	
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	virtual void Attack() override;
	virtual void ComboEnd() override;

protected:
	virtual void BeginPlay() override;
	virtual void Hit_Implementation(const FVector& impactPoint, AActor* otherActor) override;
	void PlayDeathMontageSection();
	virtual void Die() override;
	
	bool IsTargetInRange(const AActor* target, double acceptanceRadius) const;
	AActor* GetRandoPatrolActor();
	void MoveToTarget(AActor* target) const;
	void PatrolDelayFinished();
	
	UFUNCTION()
	void OnPawnSeen(APawn* pawn);

	void UpdateCombat();

private:
	void EnableHealthBarComponent(bool enabled) const;
	void LoseTargetInterest();
	void ChaseTarget();
	void AttackTarget();
};
