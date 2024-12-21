#pragma once

#include <CoreMinimal.h>
#include <Slash/Public/Items/Items.h>
#include <Weapon.generated.h>

class UBoxComponent;

UCLASS()
class SLASH_API AWeapon : public AItems {
	GENERATED_BODY()

public:
	AWeapon();
	
	void Equip(USceneComponent* parent, FName socketName, AActor* newOwner, APawn* newInstigator);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& fieldCenter);
	
	UPROPERTY(EditAnywhere, Category = Weapon)
	UBoxComponent* BoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USceneComponent* BoxTraceStartPosition;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USceneComponent* BoxTraceEndPosition;

	UPROPERTY()
	TArray<AActor*> IgnoreHitActors;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float Damage = 15.f;

	UPROPERTY(EditAnywhere, Category = Weapon)
	FVector HitBoxSize = FVector(10.f, 10.f, 10.f);
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	
private:
	bool CheckOwnerTag(FName actorTag, const AActor* actorHit) const;
};
