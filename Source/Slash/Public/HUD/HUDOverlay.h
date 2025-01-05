#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include <HUDOverlay.generated.h>

class UProgressBar;
class UTextBlock;

UCLASS()
class SLASH_API UHUDOverlay : public UUserWidget {
	GENERATED_BODY()

public:
	void Init() const;
	void SetHealthBarPercent(float percent) const;
	void SetStaminaBarPercent(float percent) const;
	void SetGold(int32 newAmount) const;
	void SetSouls(int32 newAmount) const;

private:
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* Health_pBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Stamina_pBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GoldCount_txt;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoulCount_txt;
};
