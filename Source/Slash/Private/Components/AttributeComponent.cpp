#include <Components/AttributeComponent.h>

UAttributeComponent::UAttributeComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::AddHealth(float value) {
	m_currentHealth = FMath::Clamp(m_currentHealth + value, m_currentHealth, MaxHealth); 
}

void UAttributeComponent::TakeDamage(float value) {
	m_currentHealth = FMath::Clamp(m_currentHealth - value, 0.f, MaxHealth);
}

void UAttributeComponent::BeginPlay() {
	Super::BeginPlay();

	m_currentHealth = MaxHealth;
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

