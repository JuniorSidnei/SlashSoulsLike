#include <Slash/Public/Items/Collectable.h>
#include <Kismet/GameplayStatics.h>
#include <Interfaces/Pickable.h>
#include <NiagaraFunctionLibrary.h>

int32 ACollectable::GetRandomSoulAmount() const {
	return FMath::RandRange(MinAmount, MaxAmount);
}

void ACollectable::OnSphereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
                                        UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {

	auto pickable = Cast<IPickable>(otherActor);

	if(!pickable) {
		return;
	}

	if(PickupSound) {
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
	}

	if(PickedEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickedEffect, GetActorLocation());
	}
	
	pickable->AddGold(this);
	
	Destroy();
}