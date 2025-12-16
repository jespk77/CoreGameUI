#include "PropertyWidgets.h"
#include "CoreGameUI/Elements/ToggleableButton.h"

TArray<FString> UBooleanPropertyWidget::GetPropertiesForObject() const {
	TArray<FString> names;
#if WITH_EDITORONLY_DATA
	if (PropertyClass) GetPropertyNamesForObjectWithType<FBoolProperty>(PropertyClass, names);
	else if (PropertyStruct) GetPropertyNamesForObjectWithType<FBoolProperty>(PropertyStruct, names);
#endif
	return names;
}

void UBooleanPropertyWidget::NativePreConstruct() {
	if (DisplayName.IsEmpty()) DisplayName = FText::FromString(PropertyName);
	Super::NativePreConstruct();
}

void UBooleanPropertyWidget::NativeTick(const FGeometry& geometry, float delta) {
	Super::NativeTick(geometry, delta);
	if (UpdateWidgetOnTick) UpdateWidget();
}

bool UBooleanPropertyWidget::SetValue(const bool newValue) {
	if (SetPropertyValue(newValue)) {
		UpdateWidget();
		OnValueUpdated.Broadcast(newValue);
	}
	return GetValue();
}

void UBooleanPropertyWidget::SetObjectWithType(UStruct* obj, void* data) {
	SetEditableObject(obj, data, PropertyName);
	UpdateWidget();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

TArray<FString> UNumericPropertyWidget::GetPropertiesForObject() const {
	TArray<FString> names;
#if WITH_EDITORONLY_DATA
	if (PropertyClass) GetPropertyNamesForObjectWithType<FNumericProperty>(PropertyClass, names);
	else if (PropertyStruct) GetPropertyNamesForObjectWithType<FNumericProperty>(PropertyStruct, names);
#endif
	return names;
}

void UNumericPropertyWidget::NativePreConstruct() {
	if (DisplayName.IsEmpty()) DisplayName = FText::FromString(PropertyName);
	Super::NativePreConstruct();
}

void UNumericPropertyWidget::NativeTick(const FGeometry& geometry, float delta) {
	Super::NativeTick(geometry, delta);
	if (UpdateWidgetOnTick) UpdateWidget();
}

float UNumericPropertyWidget::SetValue(const float newValue) {
	const float value = FMath::Clamp(newValue, GetMinimum(), GetMaximum());
	if (SetPropertyValue(value)) {
		UpdateWidget();
		OnValueUpdated.Broadcast(value);
	}
	return GetValue();
}

void UNumericPropertyWidget::SetObjectWithType(UStruct* obj, void* data) {
	SetEditableObject(obj, data, PropertyName);
	UpdateWidget();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

TArray<FString> UToggleNumericPropertyWidget::GetPropertiesForObject() const {
	TArray<FString> names;
#if WITH_EDITORONLY_DATA
	if (PropertyClass) GetPropertyNamesForObjectWithType<FNumericProperty>(PropertyClass, names);
	else if (PropertyStruct) GetPropertyNamesForObjectWithType<FNumericProperty>(PropertyStruct, names);
#endif
	return names;
}

TArray<FString> UToggleNumericPropertyWidget::GetConditionalPropertiesForObject() const {
	TArray<FString> names;
#if WITH_EDITORONLY_DATA
	if (PropertyClass) GetPropertyNamesForObjectWithType<FBoolProperty>(PropertyClass, names);
	else if (PropertyStruct) GetPropertyNamesForObjectWithType<FBoolProperty>(PropertyStruct, names);
#endif
	return names;
}

void UToggleNumericPropertyWidget::NativePreConstruct() {
	if (DisplayName.IsEmpty()) DisplayName = FText::FromString(PropertyName);
	Super::NativePreConstruct();
}

void UToggleNumericPropertyWidget::NativeTick(const FGeometry& geometry, float delta) {
	Super::NativeTick(geometry, delta);
	if (UpdateWidgetOnTick) UpdateWidget();
}

float UToggleNumericPropertyWidget::SetValue(const float newValue) {
	const float value = FMath::Clamp(newValue, GetMinimum(), GetMaximum());
	if (SetPropertyValue(value)) {
		UpdateWidget();
		OnValueUpdated.Broadcast(value);
	}
	return GetValue();
}

bool UToggleNumericPropertyWidget::SetEnabled(bool enabled) {
	if (SetControlValue(enabled)) {
		UpdateWidget();
		OnEnabledUpdated.Broadcast(enabled);
	}
	return GetEnabled();
}

void UToggleNumericPropertyWidget::SetObjectWithType(UStruct* obj, void* data) {
	SetEditableObject(obj, data, PropertyName, &EditConditionPropertyName);
	UpdateWidget();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void USelectionPropertyWidget::UpdateEnumProperty() {
	EnumProperty = nullptr;
	if (EditProperty) EnumProperty = UClassUtilities::FindEnumWithName(EditProperty->GetCPPType());

	if (EnumProperty) {
		const int32 max = EnumProperty->NumEnums() - 1;
		Entries.Reset(max);
		for (int32 i = 0; i < max; i++) Entries.Add(EnumProperty->GetDisplayNameTextByIndex(i));
	}
}

TArray<FString> USelectionPropertyWidget::GetPropertiesForObject() const {
	TArray<FString> names;
#if WITH_EDITORONLY_DATA
	if (PropertyClass) GetPropertyNamesForObjectWithType<FEnumProperty>(PropertyClass, names);
	else if (PropertyStruct) GetPropertyNamesForObjectWithType<FEnumProperty>(PropertyStruct, names);
#endif
	return names;
}

void USelectionPropertyWidget::NativePreConstruct() {
	if (DisplayName.IsEmpty()) DisplayName = FText::FromString(PropertyName);
#if WITH_EDITORONLY_DATA
	if (PropertyClass) EditProperty = GetPropertyFromName<FEnumProperty>(PropertyClass, PropertyName);
	else if (PropertyStruct) EditProperty = GetPropertyFromName<FEnumProperty>(PropertyStruct, PropertyName);
	else EditProperty = nullptr;
#endif
	UpdateEnumProperty();
	Super::NativePreConstruct();
}

void USelectionPropertyWidget::NativeTick(const FGeometry& geometry, float delta) {
	Super::NativeTick(geometry, delta);
	if (UpdateWidgetOnTick) UpdateWidget();
}

uint8 USelectionPropertyWidget::SetValue_uint8(const uint8 newValue) {
	if (SetPropertyValue(newValue)) {
		UpdateWidget();
		OnValueUpdated.Broadcast(Value);
	}
	return GetValue_uint8();
}

void USelectionPropertyWidget::SetObjectWithType(UStruct* obj, void* data) {
	SetEditableObject(obj, data, PropertyName);
	UpdateWidget();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UButtonSelectionPropertyWidget::UpdateEnumProperty() {
	Super::UpdateEnumProperty();

	if (!Container) return;

	for (TSharedPtr<SToggleableButton>& button : Buttons) Container->RemoveSlot(button.ToSharedRef());
	Buttons.Reset();

	if (!EnumProperty) return;
	for (int32 index = 0; index < Entries.Num(); index++) {
		TSharedPtr<SToggleableButton>& button = Buttons.AddZeroed_GetRef();
		Container->AddSlot().Padding(5.f).AutoWidth().AttachWidget(
			SAssignNew(button, SToggleableButton).Text(Entries[index])
			.OnClicked_Lambda([this, buttonValue = EnumProperty->GetValueByIndex(index)]() {
			SetValue(buttonValue);
			return FReply::Handled();
		})
		);
	}
}

void UButtonSelectionPropertyWidget::UpdateWidget() {
	Super::UpdateWidget();

	int32 index = 0;
	for (const TSharedPtr<SToggleableButton>& button : Buttons) {
		button->SetSelected(GetValue() == index++);
	}
}

void UButtonSelectionPropertyWidget::ReleaseCustomElements() {
	Buttons.Reset();
}
