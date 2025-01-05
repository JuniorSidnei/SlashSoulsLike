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
	FORCEINLINE float GetStaminaPercent() const { return m_currentStamina / MaxStamina; }
	FORCEINLINE float GetStamina() const { return m_currentStamina; }
	
	void AddHealth(float amount);
	void TakeDamage(float amount);
	void AddStamina(float amount);
	void TakeStamina(float amount);
	
	
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxStamina;

protected:
	virtual void BeginPlay() override;

private:
	float m_currentHealth;
	float m_currentStamina;
};
