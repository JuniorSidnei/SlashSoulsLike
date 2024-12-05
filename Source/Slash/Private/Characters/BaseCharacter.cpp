
#include <Characters/BaseCharacter.h>
#include <Weapon/Weapon.h>
#include <Components/BoxComponent.h>
#include <Components/AttributeComponent.h>

ABaseCharacter::ABaseCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttribueComponent"));
}

void ABaseCharacter::BeginPlay() {
	Super::BeginPlay();
}

void ABaseCharacter::Attack() { }

void ABaseCharacter::ComboEnd() { }

void ABaseCharacter::Die() { }

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type enabled) {
	if(!m_currentWeapon) return;

	m_currentWeapon->BoxCollisionComponent->SetCollisionEnabled(enabled);
	m_currentWeapon->IgnoreHitActors.Empty();
}

void ABaseCharacter::PlayHitReactMontage() const {
	auto* animInstance = GetMesh()->GetAnimInstance();

	if(!animInstance || !HitReactMontage) {
		return;
	}

	animInstance->Montage_Play(HitReactMontage);
}

void ABaseCharacter::PlayDeathMontage() {
	auto* animInstance = GetMesh()->GetAnimInstance();

	if(!animInstance || !DeathMontage) {
		return;
	}
	
	animInstance->Montage_Play(DeathMontage);
}

void ABaseCharacter::Hit_Implementation(const FVector& impactPoint) { }

void ABaseCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
