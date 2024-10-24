#pragma once

#include <CoreMinimal.h>
#include <Items.h>
#include <Weapon.generated.h>

class UBoxComponent;

UCLASS()
class SLASH_API AWeapon : public AItems {
	GENERATED_BODY()

public:
	AWeapon();
	
	void Equip(USceneComponent* parent, FName socketName);

	UPROPERTY(EditAnywhere, Category = "Weapon")
	UBoxComponent* BoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	USceneComponent* BoxTraceStartPosition;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	USceneComponent* BoxTraceEndPosition;


	TArray<AActor*> IgnoreHitActors;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void OnShpereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) override;
	virtual void OnShpereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex) override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
};
