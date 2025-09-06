#include "InputWidgets.h"
#include "Widgets/Input/SSlider.h"
#include "CoreGameUI/Elements/ToggleableButton.h"

TSharedRef<SWidget> UInputWidgetBase::RebuildWidget() {
	SAssignNew(Container, SHorizontalBox) +
		SHorizontalBox::Slot().Padding(5.f).AutoWidth()[
			SAssignNew(Label, STextBlock)
		];

	AddCustomElements();
	return Container.ToSharedRef();
}

void UInputWidgetBase::ReleaseSlateResources(bool releaseChildren) {
	Label.Reset();
	Container.Reset();
	ReleaseCustomElements();
	Super::ReleaseSlateResources(releaseChildren);
}

void UInputWidgetBase::NativePreConstruct() {
	Super::NativePreConstruct();
	UpdateWidget();
}

void UInputWidgetBase::UpdateWidget() {
	if (Label) Label->SetText(DisplayName);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UBooleanInputWidget::UpdateWidget() {
	if (Checkbox) Checkbox->SetIsChecked(GetValue() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	Super::UpdateWidget();
}

void UBooleanInputWidget::AddCustomElements() {
	Container->AddSlot().Padding(5.f).AutoWidth().AttachWidget(
		SAssignNew(Checkbox, SCheckBox)
		.OnCheckStateChanged_Lambda([&](const ECheckBoxState& state) { SetValue(state == ECheckBoxState::Checked); })
	);
}

void UBooleanInputWidget::ReleaseCustomElements() {
	Checkbox.Reset();
}

bool UBooleanInputWidget::SetValue(const bool newValue) {
	if (Value != newValue) {
		Value = newValue;
		UpdateWidget();
		OnValueUpdated.Broadcast(Value);
	}
	return GetValue();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UNumericInputWidget::UpdateWidget() {
	if (Slider) Slider->SetValue(GetValue());
	if (Text) Text->SetText(FText::FromString(FString::SanitizeFloat(GetValue())));
	Super::UpdateWidget();
}

void UNumericInputWidget::AddCustomElements() {
	Container->AddSlot().Padding(5.f).FillWidth(1).AttachWidget(
		SAssignNew(Slider, SSlider)
		.MinValue(GetMinimum()).MaxValue(GetMaximum()).StepSize(GetStep()).MouseUsesStep(true)
		.OnValueChanged_Lambda([&](const float value) { SetValue(value); })
	);

	Container->AddSlot().Padding(5.f).AutoWidth().AttachWidget(
		SAssignNew(Text, SEditableText)
		.OnTextCommitted_Lambda([&](const FText& text, ETextCommit::Type commitType) { SetValue(FCString::Atof(*text.ToString())); })
	);
}

void UNumericInputWidget::ReleaseCustomElements() {
	Slider.Reset();
	Text.Reset();
}

float UNumericInputWidget::SetValue(const float newValue) {
	if (Value != newValue) {
		Value = FMath::Clamp(newValue, Minimum, Maximum);
		UpdateWidget();
		OnValueUpdated.Broadcast(Value);
	}
	return GetValue();
}

float UNumericInputWidget::SetMinimum(const float min) {
	Minimum = FMath::Min(min, GetMaximum() - GetStep());
	if (Slider) Slider->SetMinAndMaxValues(GetMinimum(), GetMaximum());
	SetValue(Value);
	return GetMinimum();
}

float UNumericInputWidget::SetMaximum(const float max) {w
	Maximum = FMath::Max(max, GetMinimum() + GetStep());
	if (Slider) Slider->SetMinAndMaxValues(GetMinimum(), GetMaximum());
	SetValue(Value);
	return GetMaximum();
}

float UNumericInputWidget::SetStep(const float newStep) {
	Step = newStep;
	if (Slider) Slider->SetStepSize(GetStep());
	return GetStep();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UToggleNumericInputWidget::UpdateWidget() {
	if (EditCheckbox) EditCheckbox->SetIsChecked(GetEnabled() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	if (Slider) Slider->SetEnabled(GetEnabled());
	if (Text) Text->SetEnabled(GetEnabled());
	Super::UpdateWidget();
}

void UToggleNumericInputWidget::AddCustomElements() {
	Container->AddSlot().Padding(5.f).AutoWidth().AttachWidget(
		SAssignNew(EditCheckbox, SCheckBox)
		.OnCheckStateChanged_Lambda([&](const ECheckBoxState& state) { SetEnabled(state == ECheckBoxState::Checked); })
	);
	Super::AddCustomElements();
}

void UToggleNumericInputWidget::ReleaseCustomElements() {
	Super::ReleaseCustomElements();
	EditCheckbox.Reset();
}

bool UToggleNumericInputWidget::SetEnabled(const bool isEnabled) {
	if (Enabled != isEnabled) {
		Enabled = isEnabled;
		UpdateWidget();
		OnEnabledUpdated.Broadcast(Enabled);
	}
	return GetEnabled();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void USelectionInputWidget::UpdateWidget() {
	if (Text) Text->SetText(GetSelectedEntry());
	Super::UpdateWidget();
}

void USelectionInputWidget::AddCustomElements() {
	Container->AddSlot().Padding(5.f).AutoWidth().AttachWidget(
		SNew(SButton).Text(FText::FromString("<"))
		.OnClicked_Lambda([&]() { SetValue(GetValue() == 0 ? Entries.Num() - 1 : GetValue() - 1); return FReply::Handled(); })
	);

	Container->AddSlot().Padding(5.f).FillWidth(1.f).AttachWidget(
		SAssignNew(Text, STextBlock).Justification(ETextJustify::Center)
	);

	Container->AddSlot().Padding(5.f).AutoWidth().AttachWidget(
		SNew(SButton).Text(FText::FromString(">"))
		.OnClicked_Lambda([&]() { SetValue(GetValue() == Entries.Num() - 1 ? 0 : GetValue() + 1); return FReply::Handled(); })
	);
}

void USelectionInputWidget::ReleaseCustomElements() {
	Text.Reset();
}

int32 USelectionInputWidget::SetValue(const int32 newValue) {
	if (Value != newValue) {
		Value = FMath::Clamp(newValue, 0, Entries.Num());
		UpdateWidget();
		OnValueUpdated.Broadcast(Value);
	}
	return GetValue();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UButtonSelectionInputWidget::UpdateWidget() {
	int32 index = 0;
	for (const TSharedPtr<SToggleableButton>& button : Buttons)
		button->SetSelected(Value == index++);
	Super::UpdateWidget();
}

void UButtonSelectionInputWidget::AddCustomElements() {
	for (const FText& entry : Entries) {
		TSharedPtr<SToggleableButton>& button = Buttons.AddZeroed_GetRef();
		Container->AddSlot().Padding(5.f).AutoWidth().AttachWidget(
			SAssignNew(button, SToggleableButton).Text(entry)
			.OnClicked_Lambda([&]() { SetValue(Entries.IndexOfByPredicate([&](const FText& text) { return text.EqualToCaseIgnored(entry); })); return FReply::Handled(); }));
	}
}

void UButtonSelectionInputWidget::ReleaseCustomElements() {
	Buttons.Reset();
}
