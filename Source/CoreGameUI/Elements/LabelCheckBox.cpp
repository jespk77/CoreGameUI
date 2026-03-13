#include "LabelCheckBox.h"
#include "Styling/DefaultStyleCache.h"

const FCheckBoxStyle ULabelCheckBox::DefaultCheckboxStyle =
FCheckBoxStyle(UE::Slate::Private::FDefaultStyleCache::GetRuntime().GetCheckboxStyle())
.SetForegroundColor(FColor::Black).SetPressedForegroundColor(FColor::Black).SetHoveredForegroundColor(FColor::Black)
.SetCheckedForegroundColor(FColor::Black).SetCheckedHoveredForegroundColor(FColor::Black).SetCheckedPressedForegroundColor(FColor::Black).SetUndeterminedForegroundColor(FColor::Black);

TSharedRef<SWidget> ULabelCheckBox::RebuildWidget() {
	TSharedRef<SWidget> widget = Super::RebuildWidget();
	MyCheckbox->SetContent(SAssignNew(CheckboxLabel, STextBlock)
		.Justification(ETextJustify::Center).Margin(5.f)
		.Text(CheckboxText));
	return widget;
}

ULabelCheckBox::ULabelCheckBox(const FObjectInitializer& initializer) : Super(initializer) {
	bCanHaveMultipleChildren = false;
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
		WidgetStyle = DefaultCheckboxStyle;
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void ULabelCheckBox::SynchronizeProperties() {
	Super::SynchronizeProperties();
	if (CheckboxLabel.IsValid()) CheckboxLabel->SetText(CheckboxText);
}

void ULabelCheckBox::ReleaseSlateResources(bool bReleaseChildren) {
	Super::ReleaseSlateResources(bReleaseChildren);
	CheckboxLabel.Reset();
	Container.Reset();
}

void ULabelCheckBox::SetCheckboxText(const FText& label) {
	CheckboxText = label;
	if (CheckboxLabel.IsValid()) CheckboxLabel->SetText(CheckboxText);
}
