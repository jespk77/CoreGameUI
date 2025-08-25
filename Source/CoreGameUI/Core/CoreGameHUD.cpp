#include "CoreGameHUD.h"

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
