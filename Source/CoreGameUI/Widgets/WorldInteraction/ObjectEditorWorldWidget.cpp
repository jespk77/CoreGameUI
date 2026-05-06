#include "ObjectEditorWorldWidget.h"

FReply UObjectEditorWorldWidgetItemBase::NativeOnMouseButtonDown(const FGeometry& geometry, const FPointerEvent& event) {
	Super::NativeOnMouseButtonDown(geometry, event);
	return FReply::Handled(); // must return Handled() or the button up event will not trigger...
}

FReply UObjectEditorWorldWidgetItemBase::NativeOnMouseButtonUp(const FGeometry& geometry, const FPointerEvent& event) {
	const FReply reply = Super::NativeOnMouseButtonUp(geometry, event);
	if (!reply.IsEventHandled()) OnClicked.Broadcast(this);
	return reply;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UObjectEditorWorldWidgetBase::NativeDestruct() {
	DestroyCreatedWidgets();
	Super::NativeDestruct();
}

void UObjectEditorWorldWidgetBase::FinishWidgetCreation(UObjectEditorWorldWidgetItemBase* widget) {
	widget->OnClicked.AddDynamic(this, &UObjectEditorWorldWidgetBase::OnWorldButtonPressed);
	widget->AddToViewport();
	ConnectionWidgets.Add(widget);
}

void UObjectEditorWorldWidgetBase::DestroyCreatedWidgets() {
	for (UObjectEditorWorldWidgetItemBase* widget : ConnectionWidgets) {
		widget->OnClicked.RemoveDynamic(this, &UObjectEditorWorldWidgetBase::OnWorldButtonPressed);
		widget->RemoveFromParent();
		widget->Destruct();
	}
	ConnectionWidgets.Reset();
}

void UObjectEditorWorldWidgetBase::UpdateWidgetLocations() {
	for (UObjectEditorWorldWidgetItemBase* widget : ConnectionWidgets)
		widget->UpdateWorldLocation();
}
