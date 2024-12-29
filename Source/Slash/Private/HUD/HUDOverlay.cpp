#include <HUD/HUDOverlay.h>
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>

void UHUDOverlay::SetHealthBarPercent(float percent) const {
	if(!Health_pBar) return;

	Health_pBar->SetPercent(percent);
}

void UHUDOverlay::SetStaminaBarPercent(float percent) const {
	if(!Stamina_pBar) return;

	Stamina_pBar->SetPercent(percent);
}

void UHUDOverlay::SetGold(int32 newAmount) const {
	if(!GoldCount_txt) return;

	GoldCount_txt->SetText(FText::FromString(FString::Printf(TEXT("%d"), newAmount)));
}

void UHUDOverlay::SetSouls(int32 newAmount) const {
	if(!SoulCount_txt) return;

	SoulCount_txt->SetText(FText::FromString(FString::Printf(TEXT("%d"), newAmount)));
}
