#include <HUD/HealthBarWidgetComponent.h>
#include <HUD/HealthBar.h>
#include <Components/ProgressBar.h>

void UHealthBarWidgetComponent::SetHealthPercent(float percent) {

	if(m_healthBarWidget == nullptr) {
		m_healthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
	
	m_healthBarWidget->HealthProgressBar->SetPercent(percent);
}
