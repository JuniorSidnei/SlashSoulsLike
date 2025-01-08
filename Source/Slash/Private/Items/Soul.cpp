#include <Items/Soul.h>
#include <Interfaces/Pickable.h>
#include <Kismet/GameplayStatics.h>
#include <NiagaraFunctionLibrary.h>


int32 ASoul::GetRandomSoulAmount() const {
	return FMath::RandRange(MinAmount, MaxAmount);
}

void ASoul::OnSphereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
                                 UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {
	
	auto pickable = Cast<IPickable>(otherActor);
	
	if(!pickable) return;

	pickable->AddSouls(this);

	if(InteractSound) {
		UGameplayStatics::PlaySoundAtLocation(this, InteractSound, GetActorLocation());
	}

	if(PickedEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickedEffect, GetActorLocation());
	}
	
	Destroy();
}
