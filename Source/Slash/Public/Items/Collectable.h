#pragma once

#include <CoreMinimal.h>
#include <Slash/Public/Items/Items.h>
#include <Collectable.generated.h>

UCLASS()
class SLASH_API ACollectable : public AItems {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Collectable)
	int32 MinAmount;

	UPROPERTY(EditAnywhere, Category = Collectable)
	int32 MaxAmount;

	UFUNCTION()
	int32 GetRandomSoulAmount() const;
	
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* PickupSound;

protected:
	virtual void OnSphereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) override;
};
