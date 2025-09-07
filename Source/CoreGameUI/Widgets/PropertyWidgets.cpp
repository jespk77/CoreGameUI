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

void IPropertyObjectEditor::SetEditableObject(UObject* obj, const FString& propertyName) {
	if (obj) SetEditableObject(obj, GetPropertyFromName(obj->GetClass(), propertyName));
	else SetEditableObject(obj, nullptr);
}

void IPropertyObjectEditor::SetEditableObjects(const TArray<UObject*>& objects, const FString& propertyName) {
	UClass* class_ = !objects.IsEmpty() ? objects[0]->GetClass() : nullptr;
	SetEditableObjects(objects, GetPropertyFromName(class_, propertyName));
}

void IPropertyObjectEditor::SetEditableObject(UObject* obj, FProperty* property) {
	if (obj && !ensureAlwaysMsgf(property, TEXT("'property' can only be null when the object is null"))) return;

	PropertyObjects.Reset(1);
	PropertyObjects.Add(obj);
	EditProperty = property;
}

void IPropertyObjectEditor::SetEditableObjects(const TArray<UObject*>& objects, FProperty* property) {
	if (!objects.IsEmpty() && !ensureAlwaysMsgf(property, TEXT("'property' can only be null when the object is null"))) return;

	PropertyObjects.Reset(objects.Num());
	PropertyObjects.Append(objects);
	EditProperty = property;
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

	for (const FText& entry : Entries) {
		TSharedPtr<SToggleableButton>& button = Buttons.AddZeroed_GetRef();
		Container->AddSlot().Padding(5.f).AutoWidth().AttachWidget(
			SAssignNew(button, SToggleableButton).Text(entry)
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
	for (const TSharedPtr<SToggleableButton>& button : Buttons) {
		button->SetSelected(GetValue() == index++);
	}
}

void UButtonSelectionPropertyWidget::AddCustomElements() { }

void UButtonSelectionPropertyWidget::ReleaseCustomElements() {
	Buttons.Reset();
}
