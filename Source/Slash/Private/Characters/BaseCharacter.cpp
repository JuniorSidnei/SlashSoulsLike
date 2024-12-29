
#include <Characters/BaseCharacter.h>
#include <Weapon/Weapon.h>
#include <Components/BoxComponent.h>
#include <Components/AttributeComponent.h>
#include <Components/CapsuleComponent.h>

ABaseCharacter::ABaseCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttribueComponent"));
}

void ABaseCharacter::BeginPlay() {
	Super::BeginPlay();
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ABaseCharacter::Attack() { }

void ABaseCharacter::ComboEnd() { }

void ABaseCharacter::Die() {
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type enabled) {
	if(!m_currentWeapon) return;

	m_currentWeapon->BoxCollisionComponent->SetCollisionEnabled(enabled);
	m_currentWeapon->IgnoreHitActors.Empty();
}

void ABaseCharacter::StopAttackMontage() const {
	auto* animInstance = GetMesh()->GetAnimInstance();

	if(!animInstance || !AttackMontage) {
		return;
	}

	animInstance->Montage_Stop(0.1f, AttackMontage);
}

void ABaseCharacter::PlayMontage(UAnimMontage* montage) const {
	auto* animInstance = GetMesh()->GetAnimInstance();

	if(!animInstance || !montage) {
		return;
	}
	
	animInstance->Montage_Play(montage);
}

void ABaseCharacter::Hit_Implementation(const FVector& impactPoint, AActor* otherActor) {
	if(AttributeComponent->IsAlive()) {
		PlayMontage(HitReactMontage);
	} else {
		PlayMontage(DeathMontage);
		Die();
	}
}

void ABaseCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}