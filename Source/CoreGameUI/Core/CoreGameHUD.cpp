#include "CoreGameHUD.h"
#include "CoreGameWidget.h"
#include "CoreMainMenuWidget.h"

void ACoreGameHUD::BeginPlay() {
	Super::BeginPlay();

	for (const TSoftClassPtr<UCoreGameWidget>& widgetClass : MainGameWidgetClasses) {
		UCoreGameWidget* widget = CreateWidget<UCoreGameWidget>(GetOwningPlayerController(), widgetClass.LoadSynchronous());
		widget->AddToViewport(widget->ZOrder);
		MainGameWidgets.Add(widget);
	}

	if (!MainMenuWidgetClass.IsNull()) {
		MainMenuWidget = CreateWidget<UCoreMainMenuWidget>(GetOwningPlayerController(), MainMenuWidgetClass.LoadSynchronous(), "MainMenuWidget");
		MainMenuWidget->AddToViewport(MainMenuWidget->ZOrder);
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
