#pragma once

#include <CoreMinimal.h>
#include <Items/Items.h>
#include <Soul.generated.h>

class UNiagaraSystem;

UCLASS()
class SLASH_API ASoul : public AItems {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 MinAmount;

	UPROPERTY(EditAnywhere)
	int32 MaxAmount;

	UFUNCTION()
	int32 GetRandomSoulAmount() const;
	
protected:
	virtual void OnSphereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) override;
	
};
