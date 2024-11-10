#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include <Interfaces/IHitable.h>
#include <Breakable.generated.h>

class UGeometryCollectionComponent;

UCLASS()
class SLASH_API ABreakable : public AActor, public IHitable {
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual void Hit_Implementation(const FVector& impactPoint) override;
	
	ABreakable();

	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection; 

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class ACollectable>> CollectableItemsList;
	
protected:
	virtual void BeginPlay() override;

private:
	bool m_isBroken = false;
};
