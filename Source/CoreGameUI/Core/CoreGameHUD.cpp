#include "CoreGameHUD.h"
#include "CoreGameWidget.h"
#include "CoreMainMenuWidget.h"

void ACoreGameHUD::BeginPlay() {
	Super::BeginPlay();

	for (const TSoftClassPtr<UCoreGameWidget>& widgetClass : MainGameWidgetClasses) {
		if (UCoreGameWidget* widget = UCoreGameWidget::ShowWidget(GetOwningPlayerController(), widgetClass.LoadSynchronous()))
			MainGameWidgets.Add(widget);
	}

	if (!MainMenuWidgetClass.IsNull())
		MainMenuWidget = UCoreGameWidget::ShowWidget<APlayerController, UCoreMainMenuWidget>(GetOwningPlayerController(), MainMenuWidgetClass.LoadSynchronous(), "MainMenuWidget");
}

inline bool ACoreGameHUD::IsMainMenuVisible() const {
	return MainMenuWidget && MainMenuWidget->IsOpen();
}

void ACoreGameHUD::SetMainMenuVisible(bool visible) {
	if (!MainMenuWidget) return;

	if (visible) MainMenuWidget->OpenMenu();
	else MainMenuWidget->CloseMenu();
}
