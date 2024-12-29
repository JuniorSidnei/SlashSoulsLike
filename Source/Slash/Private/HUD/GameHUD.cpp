#include <HUD/GameHUD.h>
#include <Blueprint/UserWidget.h>
#include <HUD/HUDOverlay.h>

void AGameHUD::BeginPlay() {
	
	Super::BeginPlay();

	auto world = GetWorld();

	if(!world) return;

	auto controller = world->GetFirstPlayerController();

	if(!controller || !HUDOverlayClass) return;
	
	m_hudWidget = CreateWidget<UHUDOverlay>(controller, HUDOverlayClass);
	m_hudWidget->AddToViewport();
}
