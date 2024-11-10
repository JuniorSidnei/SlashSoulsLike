#include <Characters/Enemies/Enemy.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/AttributeComponent.h>
#include <HUD/HealthBarWidgetComponent.h>

AEnemy::AEnemy() {
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttribueComponent"));
	HealthBarComponent = CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("HealthBar"));
	HealthBarComponent->SetupAttachment(GetRootComponent());
}

void AEnemy::BeginPlay() {
	Super::BeginPlay();
	
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	if(HealthBarComponent) {
		HealthBarComponent->SetHealthPercent(AttributeComponent->GetHealthPercent());
	}
}

void AEnemy::Hit_Implementation(const FVector& impactPoint) {
	PlayHitReactMontage();
}

void AEnemy::PlayHitReactMontage() const {
	auto* animInstance = GetMesh()->GetAnimInstance();

	if(!animInstance || !HitReactMontage) {
		return;
	}

	animInstance->Montage_Play(HitReactMontage);
}

void AEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}