#include "PropertyWidgets.h"

template<typename PropertyType>
TArray<FProperty*> IPropertyObjectEditor::GetPropertiesForObjectWithType(UClass* class_) {
	TArray<FProperty*> properties;
	if (class_) {
		for (TFieldIterator<FProperty> it(class_); it; ++it) {
			FProperty* property = *it;
			if (property->IsA<PropertyType>()) properties.Add(property);
		}
	}
	return properties;
}

template<typename PropertyType>
TArray<FString> IPropertyObjectEditor::GetPropertyNamesForObjectWithType(UClass* class_) {
	TArray<FProperty*> properties = GetPropertiesForObjectWithType<PropertyType>(class_);
	TArray<FString> names;
	names.Reserve(properties.Num());
	for (FProperty* property : properties)
		names.Add(property->GetName());
	return names;
}

void IPropertyObjectEditor::SetEditableObject(UObject* obj, const FString& propertyName) {
	PropertyObject = obj;

	EditProperty = nullptr;
	if (PropertyObject) {
		TArray<FProperty*> properties = GetPropertiesForObjectWithType<FBoolProperty>(PropertyObject->GetClass());
		for (FProperty* property : properties) {
			if (property->GetName().Equals(propertyName, ESearchCase::IgnoreCase)) {
				EditProperty = property;
				break;
			}
		}
	}
}

template<typename ValueType>
ValueType IPropertyObjectEditor::GetPropertyValue() const {
	ValueType* value = EditProperty && PropertyObject ?
		EditProperty->ContainerPtrToValuePtr<ValueType>(PropertyObject) : nullptr;
	return value ? *value : ValueType();
}

template<typename ValueType>
bool IPropertyObjectEditor::SetPropertyValue(const ValueType& newValue) {
	if (!EditProperty || !PropertyObject) return false;

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
	return GetPropertyNamesForObjectWithType<FBoolProperty>(PropertyClass);
}

void UBooleanPropertyWidget::NativePreConstruct() {
	if (DisplayName.IsEmpty()) DisplayName = FText::FromString(PropertyName);
	Super::NativePreConstruct();
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
	return GetPropertyNamesForObjectWithType<FNumericProperty>(PropertyClass);
}

void UNumericPropertyWidget::NativePreConstruct() {
	if (DisplayName.IsEmpty()) DisplayName = FText::FromString(PropertyName);
	Super::NativePreConstruct();
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
