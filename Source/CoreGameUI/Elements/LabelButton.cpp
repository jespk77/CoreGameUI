#include "LabelButton.h"
#include "Styling/DefaultStyleCache.h"

const FButtonStyle ULabelButton::DefaultButtonStyle =
FButtonStyle(UE::Slate::Private::FDefaultStyleCache::GetRuntime().GetButtonStyle())
.SetNormalForeground(FColor::Black).SetPressedForeground(FColor::Black).SetHoveredForeground(FColor::Black);

TSharedRef<SWidget> ULabelButton::RebuildWidget() {
	TSharedRef<SWidget> widget = Super::RebuildWidget();
	MyButton->SetContent(SAssignNew(ButtonLabel, STextBlock)
		.Justification(ETextJustify::Center).Margin(5.f)
		.Text(ButtonText));
	return widget;
}

ULabelButton::ULabelButton(const FObjectInitializer& initializer) : Super(initializer) {
	bCanHaveMultipleChildren = false;
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
		WidgetStyle = DefaultButtonStyle;
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void ULabelButton::SynchronizeProperties() {
	Super::SynchronizeProperties();
	if (ButtonLabel.IsValid()) ButtonLabel->SetText(ButtonText);
}

void ULabelButton::ReleaseSlateResources(bool bReleaseChildren) {
	ButtonLabel.Reset();
	Super::ReleaseSlateResources(bReleaseChildren);
}

void ULabelButton::SetButtonText(const FText& label) {
	ButtonText = label;
	if (ButtonLabel.IsValid()) ButtonLabel->SetText(ButtonText);
}
