#include "CoreGameHUD.h"
#include "CoreGameWidget.h"
#include "CoreMainMenuWidget.h"

void ACoreGameHUD::BeginPlay() {
	Super::BeginPlay();

	if (MainGameWidgetClass) {
		MainGameWidget = CreateWidget<UCoreGameWidget>(GetOwningPlayerController(), MainGameWidgetClass, "MainGameWidget");
		MainGameWidget->AddToViewport(0);
	}

	if (MainMenuWidgetClass) {
		MainMenuWidget = CreateWidget<UCoreMainMenuWidget>(GetOwningPlayerController(), MainMenuWidgetClass, "MainMenuWidget");
		MainMenuWidget->AddToViewport(1);
	}
}

inline bool ACoreGameHUD::IsMainMenuVisible() const {
	return MainMenuWidget && MainMenuWidget->IsOpen();
}

void ACoreGameHUD::SetMainMenuVisible(bool visible) {
	if (!MainMenuWidget) return;

	if (visible) MainMenuWidget->OpenMenu();
	else MainMenuWidget->CloseMenu();
}
