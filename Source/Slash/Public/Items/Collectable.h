#pragma once

#include <CoreMinimal.h>
#include <Slash/Public/Items/Items.h>
#include <Collectable.generated.h>

UENUM(BlueprintType)
enum class ECollectableType {
	Soul,
	Treasure
};

UCLASS()
class SLASH_API ACollectable : public AItems {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* PickupSound;

	UPROPERTY(EditAnywhere)
	ECollectableType Type = ECollectableType::Treasure;

	UPROPERTY(EditAnywhere, Category = Collectable)
	int32 CollectableAmount;

protected:
	virtual void OnSphereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) override;
};
