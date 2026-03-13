#include "CoreMainMenuWidget.h"

void UCoreMainMenuWidget::OnVisibilityChange(const ESlateVisibility visible) {
	GetOwningPlayer()->SetPause(visible == ESlateVisibility::Visible);
}

void UCoreMainMenuWidget::NativeOnInitialized() {
	Super::NativeOnInitialized();
	OnVisibilityChanged.AddDynamic(this, &UCoreMainMenuWidget::OnVisibilityChange);
}

void UCoreMainMenuWidget::NativeDestruct() {
	OnVisibilityChanged.RemoveDynamic(this, &UCoreMainMenuWidget::OnVisibilityChange);
}

UCoreMainMenuWidget::UCoreMainMenuWidget(const FObjectInitializer& initializer) : Super(initializer) {
	ZOrder = 2;
}

void UCoreMainMenuWidget::OpenMenu() {
	SetVisibility(ESlateVisibility::Visible);
}

void UCoreMainMenuWidget::CloseMenu() {
	SetVisibility(ESlateVisibility::Hidden);
}

void UCoreMainMenuWidget::ExitGame(bool force) {
	GetOwningPlayer()->ConsoleCommand(force ? "quit force" : "quit");
}
