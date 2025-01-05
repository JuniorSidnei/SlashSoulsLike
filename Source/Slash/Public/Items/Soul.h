#pragma once

#include <CoreMinimal.h>
#include <Items/Items.h>
#include <Soul.generated.h>

UCLASS()
class SLASH_API ASoul : public AItems {
	GENERATED_BODY()

protected:
	
	virtual void OnSphereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) override;
	
};
