#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include <Items.generated.h>

class USphereComponent;
class USoundBase;
class UNiagaraComponent;

enum class EITemState {
	Hovering,
	Equipped
};

UCLASS()
class SLASH_API AItems : public AActor {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Amplitude = 1.0f;

	UPROPERTY(EditAnywhere)
	float TimeConstant = 1.0f;

	UPROPERTY(EditAnywhere)
	float RotationSpeed = 100.0f;
	
	AItems();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure)
	float TransformedSin() const;

	UFUNCTION(BlueprintPure)
	float TransformedCos() const;

	UFUNCTION()
	virtual void OnSphereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* sparkEffect;

protected:
	EITemState ItemState = EITemState::Hovering;

	UPROPERTY(EditAnywhere)
	USoundBase* InteractSound;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereCollision;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float m_runningTime;
};