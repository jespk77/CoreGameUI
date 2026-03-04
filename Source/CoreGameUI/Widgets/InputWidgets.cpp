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
	if (Checkbox) {
		Checkbox->SetIsChecked(GetValue() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		Checkbox->SetEnabled(GetEnabled());
	}
	Super::UpdateWidget();
}

void UBooleanInputWidget::AddCustomElements() {
	Container->AddSlot().Padding(5.f).AutoWidth().AttachWidget(
		SAssignNew(Checkbox, SCheckBox)
		.IsEnabled(GetEnabled())
		.OnCheckStateChanged_Lambda([&](const ECheckBoxState& state) { SetValue(state == ECheckBoxState::Checked); })
	);
}

void UBooleanInputWidget::ReleaseCustomElements() {
	Checkbox.Reset();
}

bool UBooleanInputWidget::SetValue(const bool newValue) {
	if (GetEnabled() && Value != newValue) {
		Value = newValue;
		UpdateWidget();
		OnValueUpdated.Broadcast(Value);
	}
	return GetValue();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UNumericInputWidget::UpdateWidget() {
	const bool enabled = GetEnabled();
	const float value = GetValue();

	if (Slider) {
		Slider->SetValue(value);
		Slider->SetEnabled(enabled);
	}

	if (Text) {
		Text->SetText(FText::FromString(FString::SanitizeFloat(value)));
		Text->SetEnabled(enabled);
	}

	Super::UpdateWidget();
}

void UNumericInputWidget::AddCustomElements() {
	Container->AddSlot().Padding(5.f).FillWidth(1).MinWidth(100.f).AttachWidget(
		SAssignNew(Slider, SSlider)
		.IsEnabled(GetEnabled())
		.MinValue(GetMinimum()).MaxValue(GetMaximum()).StepSize(GetStep()).MouseUsesStep(true)
		.OnValueChanged_Lambda([&](const float value) { SetValue(value); })
	);

	Container->AddSlot().Padding(5.f).AutoWidth().AttachWidget(
		SAssignNew(Text, SEditableText)
		.IsReadOnly(GetEnabled())
		.OnTextCommitted_Lambda([&](const FText& text, ETextCommit::Type commitType) { SetValue(FCString::Atof(*text.ToString())); })
	);
}

void UNumericInputWidget::ReleaseCustomElements() {
	Slider.Reset();
	Text.Reset();
}

float UNumericInputWidget::SetValue(const float newValue) {
	if (GetEnabled() && Value != newValue) {
		Value = FMath::Clamp(newValue, Minimum, Maximum);
		UpdateWidget();
		OnValueUpdated.Broadcast(Value);
	}
	return GetValue();
}

float UNumericInputWidget::SetMinimum(const float min) {
	if (GetEnabled()) {
		Minimum = FMath::Min(min, GetMaximum() - GetStep());
		if (Slider) Slider->SetMinAndMaxValues(GetMinimum(), GetMaximum());
		SetValue(Value);
	}
	return GetMinimum();
}

float UNumericInputWidget::SetMaximum(const float max) {
	if (GetEnabled()) {
		Maximum = FMath::Max(max, GetMinimum() + GetStep());
		if (Slider) Slider->SetMinAndMaxValues(GetMinimum(), GetMaximum());
		SetValue(Value);
	}
	return GetMaximum();
}

float UNumericInputWidget::SetStep(const float newStep) {
	if (GetEnabled()) {
		Step = newStep;
		if (Slider) Slider->SetStepSize(GetStep());
	}
	return GetStep();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UToggleNumericInputWidget::UpdateWidget() {
	const bool enabled = GetEnabled();
	if (EditCheckbox) EditCheckbox->SetIsChecked(enabled ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	if (Slider) Slider->SetEnabled(enabled);
	if (Text) Text->SetEnabled(enabled);
	Super::UpdateWidget();
}

void UToggleNumericInputWidget::AddCustomElements() {
	Container->AddSlot().Padding(5.f).AutoWidth().AttachWidget(
		SAssignNew(EditCheckbox, SCheckBox)
		.IsEnabled(!IsReadOnly)
		.OnCheckStateChanged_Lambda([&](const ECheckBoxState& state) { SetEnabled(state == ECheckBoxState::Checked); })
	);
	Super::AddCustomElements();
}

void UToggleNumericInputWidget::ReleaseCustomElements() {
	Super::ReleaseCustomElements();
	EditCheckbox.Reset();
}

bool UToggleNumericInputWidget::SetEnabled(const bool isEnabled) {
	if (IsReadOnly) return false;

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
		.IsEnabled(GetEnabled())
		.OnClicked_Lambda([&]() { SetValue(GetValue() == 0 ? Entries.Num() - 1 : GetValue() - 1); return FReply::Handled(); })
	);

	Container->AddSlot().Padding(5.f).FillWidth(1.f).AttachWidget(
		SAssignNew(Text, STextBlock).Justification(ETextJustify::Center)
	);

	Container->AddSlot().Padding(5.f).AutoWidth().AttachWidget(
		SNew(SButton).Text(FText::FromString(">"))
		.IsEnabled(GetEnabled())
		.OnClicked_Lambda([&]() { SetValue(GetValue() == Entries.Num() - 1 ? 0 : GetValue() + 1); return FReply::Handled(); })
	);
}

void USelectionInputWidget::ReleaseCustomElements() {
	Text.Reset();
}

int32 USelectionInputWidget::SetValue(const int32 newValue) {
	if (GetEnabled() && Value != newValue) {
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
			.IsEnabled(GetEnabled())
			.OnClicked_Lambda([&]() { SetValue(Entries.IndexOfByPredicate([&](const FText& text) { return text.EqualToCaseIgnored(entry); })); return FReply::Handled(); }));
	}
}

void UButtonSelectionInputWidget::ReleaseCustomElements() {
	Buttons.Reset();
}
