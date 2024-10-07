#include <Weapon/Weapon.h>

void AWeapon::Equip(USceneComponent* parent, FName socketName) {
	// Create transform rules
	FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, false);

	// Attach mesh in parent in the socket transform with transform rules
	mesh->AttachToComponent(parent, transformRules, socketName);
}

void AWeapon::OnShpereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
                                   UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {
	Super::OnShpereStartOverlap(overlappedComponent, otherActor, otherComp, otherBodyIndex, bFromSweep, sweepResult);
}

void AWeapon::OnShpereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex) {
	Super::OnShpereEndOverlap(overlappedComponent, otherActor, otherComp, otherBodyIndex);
}
