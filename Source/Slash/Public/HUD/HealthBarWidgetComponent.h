#pragma once

#include <CoreMinimal.h>
#include <Components/WidgetComponent.h>
#include <HealthBarWidgetComponent.generated.h>

class UHealthBar;

UCLASS()
class SLASH_API UHealthBarWidgetComponent : public UWidgetComponent {
	GENERATED_BODY()

public:
	void SetHealthPercent(float percent);

private:
	UPROPERTY()
	UHealthBar* m_healthBarWidget;
};
