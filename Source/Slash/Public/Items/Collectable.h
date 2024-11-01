#pragma once

#include <CoreMinimal.h>
#include <Slash/Public/Items/Items.h>
#include <Collectable.generated.h>

UCLASS()
class SLASH_API ACollectable : public AItems {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* PickupSound;
	
protected:
	virtual void OnShpereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) override;
};
