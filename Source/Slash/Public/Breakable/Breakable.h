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

	virtual void Hit(const FVector& impactPoint) override;
	
	ABreakable();

	UPROPERTY()
	UGeometryCollectionComponent* GeometryCollection;
	
protected:
	virtual void BeginPlay() override;
};
