#include "PropertyWidgets.h"
#include "BlueprintUtilities/BlueprintFunctionLibrary/ClassUtilities.h"

template<typename PropertyType>
void IPropertyObjectEditor::GetPropertiesForObjectWithType(UClass* class_, TArray<FProperty*>& properties) {
	static TFunctionRef<bool(FProperty*)> checkFunction = [&](FProperty* property) { return property->IsA<PropertyType>(); };
	UClassUtilities::GetAllPropertiesForClass(properties, class_, &checkFunction);
}

template<typename PropertyType>
void IPropertyObjectEditor::GetPropertyNamesForObjectWithType(UClass* class_, TArray<FString>& names) {
	static TFunctionRef<bool(FProperty*)> checkFunction = [&](FProperty* property) { return property->IsA<PropertyType>(); };
	UClassUtilities::GetAllPropertyNamesForClass(names, class_, &checkFunction);
}

template<typename PropertyType>
void IPropertyObjectEditor::SetPropertyFromName(UClass* class_, const FString& propertyName) {
	EditProperty = nullptr;
	if (class_) {
		TArray<FProperty*> properties;
		GetPropertiesForObjectWithType<PropertyType>(class_, properties);
		for (FProperty* property : properties) {
			if (property->GetName().Equals(propertyName, ESearchCase::IgnoreCase)) {
				EditProperty = property;
				break;
			}
		}
	}
}

void IPropertyObjectEditor::SetEditableObject(UObject* obj, const FString& propertyName) {
	PropertyObject = obj;
	SetPropertyFromName(PropertyObject ? PropertyObject->GetClass() : nullptr, propertyName);
}

template<typename ValueType>
ValueType IPropertyObjectEditor::GetPropertyValue() const {
	ValueType* value = EditProperty && PropertyObject ?
		EditProperty->ContainerPtrToValuePtr<ValueType>(PropertyObject) : nullptr;
	return value ? *value : ValueType();
}

template<typename ValueType>
bool IPropertyObjectEditor::SetPropertyValue(const ValueType& newValue) {
	if (!EditProperty || !IsValid(PropertyObject)) return false;

	ValueType* value = EditProperty->ContainerPtrToValuePtr<ValueType>(PropertyObject);
	if (value) {
		const ValueType previousValue = *value;
		*value = newValue;
		return previousValue != newValue;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

TArray<FString> UBooleanPropertyWidget::GetPropertiesForObject() const {
	TArray<FString> names;
	GetPropertyNamesForObjectWithType<FBoolProperty>(PropertyClass, names);
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

void UBooleanPropertyWidget::SetObject(UObject* obj) {
	SetEditableObject(obj, PropertyName);
	UpdateWidget();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

TArray<FString> UNumericPropertyWidget::GetPropertiesForObject() const {
	TArray<FString> names;
	GetPropertyNamesForObjectWithType<FNumericProperty>(PropertyClass, names);
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
	if (SetPropertyValue(newValue)) {
		UpdateWidget();
		OnValueUpdated.Broadcast(newValue);
	}
	return GetValue();
}

void UNumericPropertyWidget::SetObject(UObject* obj) {
	SetEditableObject(obj, PropertyName);
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
	GetPropertyNamesForObjectWithType<FEnumProperty>(PropertyClass, names);
	return names;
}

void USelectionPropertyWidget::NativePreConstruct() {
	if (DisplayName.IsEmpty()) DisplayName = FText::FromString(PropertyName);
	if (!PropertyObject) SetPropertyFromName<FEnumProperty>(PropertyClass, PropertyName);
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

void USelectionPropertyWidget::SetObject(UObject* obj) {
	SetEditableObject(obj, PropertyName);
	UpdateEnumProperty();
	UpdateWidget();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UButtonSelectionPropertyWidget::UpdateEnumProperty() {
	Super::UpdateEnumProperty();

	for (TSharedPtr<SButton>& button : Buttons) Container->RemoveSlot(button.ToSharedRef());
	Buttons.Reset();

	for (const FText& entry : Entries) {
		TSharedPtr<SButton>& button = Buttons.AddZeroed_GetRef();
		Container->AddSlot().Padding(5.f).AutoWidth().AttachWidget(
			SAssignNew(button, SButton).Text(entry)
			.ButtonStyle(&UnselectedStyle)
			.OnClicked_Lambda([&]() {
			if (EnumProperty) {
				int64 value = EnumProperty->GetValueByName(FName(entry.ToString()));
				SetValue(value);
			}
			return FReply::Handled();
		})
		);
	}
}

void UButtonSelectionPropertyWidget::UpdateWidget() {
	Super::UpdateWidget();

	int32 index = 0;
	for (const TSharedPtr<SButton>& button : Buttons) {
		button->SetButtonStyle(GetValue() == index++ ? &SelectedStyle : &UnselectedStyle);
	}
}

void UButtonSelectionPropertyWidget::AddCustomElements() { }

void UButtonSelectionPropertyWidget::ReleaseCustomElements() {
	Buttons.Reset();
}
