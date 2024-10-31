#include <Characters/Enemies/Enemy.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/CapsuleComponent.h>

AEnemy::AEnemy() {
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemy::BeginPlay() {
	Super::BeginPlay();
	
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AEnemy::Hit_Implementation(const FVector& impactPoint) {
	PlayHitReactMontage();
}

void AEnemy::PlayHitReactMontage() {
	auto* animInsatnce = GetMesh()->GetAnimInstance();

	if(!animInsatnce || !HitReactMontage) {
		return;
	}

	animInsatnce->Montage_Play(HitReactMontage);
}

void AEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

