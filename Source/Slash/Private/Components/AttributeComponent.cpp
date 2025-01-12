#include <Components/AttributeComponent.h>

UAttributeComponent::UAttributeComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::AddHealth(float amount) {
	m_currentHealth = FMath::Clamp(m_currentHealth + amount, m_currentHealth, MaxHealth); 
}

void UAttributeComponent::TakeDamage(float amount) {
	m_currentHealth = FMath::Clamp(m_currentHealth - amount, 0.f, MaxHealth);
}

void UAttributeComponent::AddStamina(float amount) {
	m_currentStamina = FMath::Clamp(m_currentStamina + amount, m_currentStamina, MaxStamina);
}

void UAttributeComponent::TakeStamina(float amount) {
	m_currentStamina = FMath::Clamp(m_currentStamina - amount, 0, MaxStamina);
}

void UAttributeComponent::TakeDodgeStamina() {
	TakeStamina(DodgeStaminaCost);
}

void UAttributeComponent::BeginPlay() {
	Super::BeginPlay();

	m_currentHealth = MaxHealth;
	m_currentStamina = MaxStamina;
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	m_currentStamina += DeltaTime * StaminaRecoveryAmount;
	m_currentStamina = FMath::Clamp(m_currentStamina, 0, MaxStamina);
}

