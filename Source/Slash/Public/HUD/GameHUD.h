#pragma once

#include <CoreMinimal.h>
#include <GameFramework/HUD.h>
#include <GameHUD.generated.h>

class UHUDOverlay;

UCLASS()
class SLASH_API AGameHUD : public AHUD {
	GENERATED_BODY()

public:
	FORCEINLINE UHUDOverlay* GetHUDOverlay() const { return m_hudWidget; }
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<UHUDOverlay> HUDOverlayClass;

	UPROPERTY()
	UHUDOverlay* m_hudWidget;
};
