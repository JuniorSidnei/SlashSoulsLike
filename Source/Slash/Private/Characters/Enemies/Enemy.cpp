#include <Characters/Enemies/Enemy.h>
#include <AIController.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/CapsuleComponent.h>
#include <HUD/HealthBarWidgetComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Navigation/PathFollowingComponent.h>
#include <Components/AttributeComponent.h>

#include "Weapon/Weapon.h"

AEnemy::AEnemy() {
	PrimaryActorTick.bCanEverTick = true;
	
	HealthBarComponent = CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("HealthBar"));
	HealthBarComponent->SetupAttachment(GetRootComponent());
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensingComponent->SightRadius = SightRadius;
	PawnSensingComponent->SetPeripheralVisionAngle(PeripheralVisionAngle); 
}

void AEnemy::BeginPlay() {
	Super::BeginPlay();

	Tags.Add(FName("Enemy"));
	
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	HealthBarComponent->SetVisibility(false);
	
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::OnPawnSeen);
	
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(GetRandoPatrolActor());

	UWorld* world = GetWorld();
	if(!world || !Weapon) {
		return;
	}

	auto weapon = world->SpawnActor<AWeapon>(Weapon);
	weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	m_currentWeapon = weapon;
}

void AEnemy::Hit_Implementation(const FVector& impactPoint) {
	if(AttributeComponent->IsAlive()) {
		HealthBarComponent->SetVisibility(true);
		PlayHitReactMontage();
	} else {
		PlayDeathMontage();
		Die();
	}
}

void AEnemy::PlayDeathMontage() {
	Super::PlayDeathMontage();

	const int32 randomSection = FMath::RandRange(0, 1);
	FString sectionString = FString::Printf(TEXT("Death_%d"), randomSection);
	FName sectionName = FName(*sectionString);

	switch (randomSection) {
		case 0:
			EnemyDeathPose = EEnemyDeathPose::DeadPoseOne;
			break;
		case 1:
			EnemyDeathPose = EEnemyDeathPose::DeadPoseTwo;
			break;
		default:
			EnemyDeathPose = EEnemyDeathPose::DeadPoseOne;
	}

	auto* animInstance = GetMesh()->GetAnimInstance();
	animInstance->Montage_JumpToSection(sectionName);
}

void AEnemy::Die() {
	HealthBarComponent->SetVisibility(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(2.0f); 
}

void AEnemy::PlayHitReactMontage() const {
	Super::PlayHitReactMontage();
}

bool AEnemy::IsTargetInRange(const AActor* target, double acceptanceRadius) const {
	if(!target) return false;
	
	const double targetDistance = (target->GetActorLocation() - GetActorLocation()).Size();
	return targetDistance <= acceptanceRadius;
}

AActor* AEnemy::GetRandoPatrolActor() {
	
	TArray<AActor*> validTargets;
	
	for (auto& targetPoint : PatrolPoints) {
		if(!PatrolTarget) {
			validTargets.AddUnique(targetPoint);
		}
		else if(targetPoint != PatrolTarget) {
			validTargets.AddUnique(targetPoint);
		}	
	}
	
	const int32 maxPatrolPoints = validTargets.Num();

	if(maxPatrolPoints <= 0) {
		return nullptr;
	}
		
	const int32 randomSection = FMath::RandRange(0, maxPatrolPoints - 1);
	
	if(!PatrolTarget) {
		PatrolTarget = validTargets[randomSection];
	}
	
	return validTargets[randomSection];
}

void AEnemy::MoveToTarget(AActor* target) const {
	if(!target) {
		return;
	}

	if (EnemyController) {
		EPathFollowingRequestResult::Type Result = EnemyController->MoveToActor(target);
		if (Result != EPathFollowingRequestResult::RequestSuccessful) {
			UE_LOG(LogTemp, Error, TEXT("MoveTo failed for %s: %d"), *target->GetName(), static_cast<int32>(Result));
		}
	}
}

void AEnemy::PatrolDelayFinished() {
	EnemyController->StopMovement();
	
	if(auto randomPatrolPoint = GetRandoPatrolActor()) {
		State = EEnemyState::Patrolling;
		PatrolTarget = randomPatrolPoint;
		MoveToTarget(PatrolTarget);
	}
}

void AEnemy::OnPawnSeen(APawn* pawn) {
	if(!pawn->ActorHasTag(FName("Player")) || State == EEnemyState::Attacking) {
		 return;
	}

	HealthBarComponent->SetVisibility(true);
	State = EEnemyState::ChasingTarget;
	GetWorldTimerManager().ClearTimer(PatrolDelayTimer);
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	Target = pawn;

	if(State != EEnemyState::Attacking) {
		State = EEnemyState::ChasingTarget;
		MoveToTarget(Target);	
	}
}

void AEnemy::UpdateCombat() {
	if(!IsTargetInRange(Target, SightDistance)) {
		Target = nullptr;
		HealthBarComponent->SetVisibility(false);
		State = EEnemyState::Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		MoveToTarget(GetRandoPatrolActor());
	} else if(!IsTargetInRange(Target, AttackDistance) && State != EEnemyState::ChasingTarget) {
		State = EEnemyState::ChasingTarget;
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		MoveToTarget(Target);
	} else if(IsTargetInRange(Target, AttackDistance) && State != EEnemyState::Attacking) {
		State = EEnemyState::Attacking;
		Attack();
	}
}

void AEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	if(State == EEnemyState::ChasingTarget) {
		UpdateCombat();
	}

	if(State == EEnemyState::Patrolling) {
		if(IsTargetInRange(PatrolTarget, PatrolSightDistance)) {
			State = EEnemyState::Waiting;
			PatrolTarget = nullptr;
			GetWorldTimerManager().SetTimer(PatrolDelayTimer, this, &AEnemy::PatrolDelayFinished, FMath::RandRange(MinPatrolDelayTime, MaxPatrolDelayTime));
		}	
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser) {

	if(AttributeComponent == nullptr) { return 0.0f; }
	
	AttributeComponent->TakeDamage(DamageAmount);
	HealthBarComponent->SetHealthPercent(AttributeComponent->GetHealthPercent());

	Target = EventInstigator->GetPawn();
	PatrolTarget = nullptr;

	State = EEnemyState::ChasingTarget;
	MoveToTarget(Target);
	
	return DamageAmount;
}

void AEnemy::Destroyed() {
	if(m_currentWeapon){
		m_currentWeapon->Destroy();
	}
}

void AEnemy::Attack() {
	Super::Attack();
	PlayAttackMontage();
}

void AEnemy::PlayAttackMontage() {
	Super::PlayAttackMontage();
}
