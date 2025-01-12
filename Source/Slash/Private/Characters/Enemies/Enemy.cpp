#include <Characters/Enemies/Enemy.h>
#include <AIController.h>
#include <Components/SkeletalMeshComponent.h>
#include <HUD/HealthBarWidgetComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/AttributeComponent.h>
#include <Weapon/Weapon.h>

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

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	EnableHealthBarComponent(false);
	
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

void AEnemy::Hit_Implementation(const FVector& impactPoint, AActor* otherActor) {
	Super::Hit_Implementation(impactPoint, otherActor);
	
	if(AttributeComponent->IsAlive()) {
		EnableHealthBarComponent(true);
	}

	StopAttackMontage();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorldTimerManager().ClearTimer(PatrolDelayTimer);
	GetWorldTimerManager().ClearTimer(AttackDelayTimer);
}

void AEnemy::PlayDeathMontageSection() {
	PlayMontage(DeathMontage);

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
	Super::Die();
	
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->bOrientRotationToMovement = false;
	State = EEnemyState::Dead;
	GetWorldTimerManager().ClearTimer(AttackDelayTimer);
	EnableHealthBarComponent(false);

	for (auto dropItem : DropItemsList)  {
		GetWorld()->SpawnActor<AItems>(dropItem, GetActorLocation(), GetActorRotation());	
	}
	
	SetLifeSpan(2.0f); 
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
	if(State == EEnemyState::Dead) { return; }
	
	if(!pawn->ActorHasTag(FName("Player")) || State == EEnemyState::Attacking) {
		 return;
	}

	EnableHealthBarComponent(true);
	GetWorldTimerManager().ClearTimer(PatrolDelayTimer);
	Target = pawn;
	State = EEnemyState::Engaged;
}

void AEnemy::UpdateCombat() {
	if(!IsTargetInRange(Target, SightDistance)) {
		LoseTargetInterest();
	} else if(!IsTargetInRange(Target, AttackDistance) && State != EEnemyState::ChasingTarget) {
		ChaseTarget();
	} else if(IsTargetInRange(Target, AttackDistance) && State != EEnemyState::Attacking) {
		AttackTarget();
	}
}

void AEnemy::EnableHealthBarComponent(bool enabled) const {
	if(HealthBarComponent) {
		HealthBarComponent->SetVisibility(enabled);
	}
}

void AEnemy::LoseTargetInterest() {
	State = EEnemyState::Patrolling;
	Target = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(AttackDelayTimer);
	EnableHealthBarComponent(false);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	MoveToTarget(GetRandoPatrolActor());
}

void AEnemy::ChaseTarget() {
	GetWorldTimerManager().ClearTimer(AttackDelayTimer);
	State = EEnemyState::ChasingTarget;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	MoveToTarget(Target);
}

void AEnemy::AttackTarget() {
	State = EEnemyState::Attacking;
	const auto attackDelayTime = FMath::RandRange(MinAttackDelayTime, MaxAttackDelayTime);
	GetWorldTimerManager().SetTimer(AttackDelayTimer, this, &AEnemy::Attack,attackDelayTime);
}

void AEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	if(State == EEnemyState::Dead) return;
	
	if(State == EEnemyState::Engaged || State == EEnemyState::ChasingTarget) {
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
	
	if(HealthBarComponent) {
		HealthBarComponent->SetHealthPercent(AttributeComponent->GetHealthPercent());
	}
	
	Target = EventInstigator->GetPawn();
	PatrolTarget = nullptr;

	ChaseTarget();
	
	return DamageAmount;
}

void AEnemy::Destroyed() {
	if(m_currentWeapon) {
		m_currentWeapon->Destroy();
	}
}

void AEnemy::Attack() {
	Super::Attack();
	PlayMontage(AttackMontage);
}

void AEnemy::ComboEnd() {
	Super::ComboEnd();

	if(Target->ActorHasTag(FName("CharacterDead"))) {
		LoseTargetInterest();
		return;
	}
	
	State = EEnemyState::Engaged;
}

FVector AEnemy::GetTranslationWarpTarget() {
	if(!Target) { return FVector(); }
	
	const auto targetLocation = Target->GetActorLocation();
	auto targetDist = (GetActorLocation() - targetLocation).GetSafeNormal();
	targetDist *= WarpTargetDistance;

	return targetLocation + targetDist; 
}

FVector AEnemy::GetRotationWartTarget() {
	if(!Target) { return FVector(); }

	return Target->GetActorLocation();
}
