#include <Slash/Public/Items/Collectable.h>
#include <Characters/BarbarousPlayer.h>
#include <Kismet/GameplayStatics.h>

void ACollectable::OnSphereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
                                        UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {

	auto barbarousPlayer = 	Cast<ABarbarousPlayer>(otherActor);

	if(!barbarousPlayer) {
		return;
	}

	if(!PickupSound) {
		return;
	}
	
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());

	switch (Type) {
		case ECollectableType::Soul:
			break;
		case ECollectableType::Treasure:
			break;
		default: ;
	}
	
	Destroy();
}