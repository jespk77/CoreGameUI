#include "PropertyWidgets.h"
#include "BlueprintUtilities/BlueprintFunctionLibrary/ClassUtilities.h"
#include "CoreGameUI/Elements/ToggleableButton.h"

template<typename PropertyType>
void IPropertyObjectEditor::GetPropertiesForObjectWithType(UClass* class_, TArray<FProperty*>& properties) {
	UClassUtilities::GetAllPropertiesForClass<PropertyType>(properties, class_);
}

template<typename PropertyType>
void IPropertyObjectEditor::GetPropertyNamesForObjectWithType(UClass* class_, TArray<FString>& names) {
	UClassUtilities::GetAllPropertyNamesForClass<PropertyType>(names, class_);
}

template<typename PropertyType>
FProperty* IPropertyObjectEditor::GetPropertyFromName(UClass* class_, const FString& propertyName) const {
	if (class_) {
		TArray<FProperty*> properties;
		GetPropertiesForObjectWithType<PropertyType>(class_, properties);
		for (FProperty* property : properties) {
			if (property->GetName().Equals(propertyName, ESearchCase::IgnoreCase))
				return property;
		}
	}

	return nullptr;
}

template<typename ValueType>
ValueType IPropertyObjectEditor::GetPropertyValue() const {
	ValueType* value = EditProperty && !PropertyObjects.IsEmpty() ?
		EditProperty->ContainerPtrToValuePtr<ValueType>(PropertyObjects[0]) : nullptr;
	return value ? *value : ValueType();
}

template<typename ValueType>
bool IPropertyObjectEditor::SetPropertyValue(const ValueType& newValue) {
	if (!EditProperty) return false;

	bool result = false;
	for (UObject* obj : PropertyObjects) {
		ValueType* value = EditProperty->ContainerPtrToValuePtr<ValueType>(obj);
		if (value) {
			const ValueType previousValue = *value;
			*value = newValue;
			if (previousValue != newValue) result = true;
		}
	}

	return result;
}

template<typename ValueType>
ValueType IPropertyObjectEditor::GetControlValue() const {
	ValueType* value = ControlProperty ? ControlProperty->ContainerPtrToValuePtr<ValueType>(PropertyObjects[0]) : nullptr;
	return value ? *value : ValueType();
}

template<typename ValueType>
bool IPropertyObjectEditor::SetControlValue(const ValueType& newValue) {
	if (!ControlProperty) return false;

	bool result = false;
	for (UObject* obj : PropertyObjects) {
		ValueType* value = ControlProperty->ContainerPtrToValuePtr<ValueType>(obj);
		if (value) {
			const ValueType previousValue = *value;
			*value = newValue;
			if (previousValue != newValue) result = true;
		}
	}

	return result;
}

void IPropertyObjectEditor::SetEditableObject(UObject* obj, const FString& propertyName, const FString* controlPropertyName) {
	if (obj) SetEditableObject(obj, GetPropertyFromName(obj->GetClass(), propertyName), controlPropertyName ? GetPropertyFromName(obj->GetClass(), *controlPropertyName) : nullptr);
	else SetEditableObject(obj, nullptr);
}

void IPropertyObjectEditor::SetEditableObjects(const TArray<UObject*>& objects, const FString& propertyName, const FString* controlPropertyName) {
	UClass* class_ = !objects.IsEmpty() ? objects[0]->GetClass() : nullptr;
	SetEditableObjects(objects, GetPropertyFromName(class_, propertyName), controlPropertyName ? GetPropertyFromName(class_, *controlPropertyName) : nullptr);
}

void IPropertyObjectEditor::SetEditableObject(UObject* obj, FProperty* property, FProperty* controlProperty) {
	if (obj && !ensureAlwaysMsgf(property, TEXT("'property' can only be null when the object is null"))) return;

	PropertyObjects.Reset(1);
	PropertyObjects.Add(obj);
	EditProperty = property;
	ControlProperty = controlProperty;
}

void IPropertyObjectEditor::SetEditableObjects(const TArray<UObject*>& objects, FProperty* property, FProperty* controlProperty) {
	if (!objects.IsEmpty() && !ensureAlwaysMsgf(property, TEXT("'property' can only be null when the object is null"))) return;

	PropertyObjects.Reset(objects.Num());
	PropertyObjects.Append(objects);
	EditProperty = property;
	ControlProperty = controlProperty;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

TArray<FString> UBooleanPropertyWidget::GetPropertiesForObject() const {
	TArray<FString> names;
#if WITH_EDITORONLY_DATA
	GetPropertyNamesForObjectWithType<FBoolProperty>(PropertyClass, names);
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

void UBooleanPropertyWidget::SetObject_Implementation(UObject* obj) {
	SetEditableObject(obj, PropertyName);
	UpdateWidget();
}

void UBooleanPropertyWidget::SetObjects_Implementation(const TArray<UObject*>& objects) {
	SetEditableObjects(objects, PropertyName);
	UpdateWidget();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

TArray<FString> UNumericPropertyWidget::GetPropertiesForObject() const {
	TArray<FString> names;
#if WITH_EDITORONLY_DATA
	GetPropertyNamesForObjectWithType<FNumericProperty>(PropertyClass, names);
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

void UNumericPropertyWidget::SetObject_Implementation(UObject* obj) {
	SetEditableObject(obj, PropertyName);
	UpdateWidget();
}

void UNumericPropertyWidget::SetObjects_Implementation(const TArray<UObject*>& objects) {
	SetEditableObjects(objects, PropertyName);
	UpdateWidget();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

TArray<FString> UToggleNumericPropertyWidget::GetPropertiesForObject() const {
	TArray<FString> names;
#if WITH_EDITORONLY_DATA
	GetPropertyNamesForObjectWithType<FNumericProperty>(PropertyClass, names);
#endif
	return names;
}

TArray<FString> UToggleNumericPropertyWidget::GetConditionalPropertiesForObject() const {
	TArray<FString> names;
#if WITH_EDITORONLY_DATA
	GetPropertyNamesForObjectWithType<FBoolProperty>(PropertyClass, names);
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

void UToggleNumericPropertyWidget::SetObject_Implementation(UObject* obj) {
	SetEditableObject(obj, PropertyName, &EditConditionPropertyName);
	UpdateWidget();
}

void UToggleNumericPropertyWidget::SetObjects_Implementation(const TArray<UObject*>& objects) {
	SetEditableObjects(objects, PropertyName, &EditConditionPropertyName);
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
	GetPropertyNamesForObjectWithType<FEnumProperty>(PropertyClass, names);
#endif
	return names;
}

void USelectionPropertyWidget::NativePreConstruct() {
	if (DisplayName.IsEmpty()) DisplayName = FText::FromString(PropertyName);
#if WITH_EDITORONLY_DATA
	if (!PropertyObjects.IsEmpty()) EditProperty = GetPropertyFromName<FEnumProperty>(PropertyClass, PropertyName);
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

void USelectionPropertyWidget::SetObject_Implementation(UObject* obj) {
	SetEditableObject(obj, PropertyName);
	UpdateEnumProperty();
	UpdateWidget();
}

void USelectionPropertyWidget::SetObjects_Implementation(const TArray<UObject*>& objects) {
	SetEditableObjects(objects, PropertyName);
	UpdateEnumProperty();
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
		//for (const FText& entry : Entries) {
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
