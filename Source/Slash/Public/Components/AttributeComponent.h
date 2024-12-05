#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include <AttributeComponent.generated.h>


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UAttributeComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UAttributeComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	FORCEINLINE float GetHealthPercent() const { return m_currentHealth / MaxHealth; }
	FORCEINLINE float GetHealth() const { return m_currentHealth; }
	FORCEINLINE bool IsAlive() const {return m_currentHealth > 0.f; }
	
	void AddHealth(float value);
	void TakeDamage(float value);
	
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxHealth;

protected:
	virtual void BeginPlay() override;

private:
	float m_currentHealth;
};
