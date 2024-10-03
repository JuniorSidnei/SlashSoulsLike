#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items.generated.h"

class USphereComponent;

UCLASS()
class SLASH_API AItems : public AActor {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Amplitude;

	UPROPERTY(EditAnywhere)
	float TimeConstant;
	
	AItems();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure)
	float TransformedSin() const;

	UFUNCTION(BlueprintPure)
	float TransformedCos() const;

	UFUNCTION()
	void OnShpereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnShpereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float m_runningTime;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_mesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* m_sphereCollision;
};