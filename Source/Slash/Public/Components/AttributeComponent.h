#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include <AttributeComponent.generated.h>


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UAttributeComponent : public UActorComponent {
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	FORCEINLINE float GetHealthPercent() const { return CurrentHealth / MaxHealth; }

	UAttributeComponent();
	
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxHealth;

protected:
	virtual void BeginPlay() override;

	float CurrentHealth;
};
