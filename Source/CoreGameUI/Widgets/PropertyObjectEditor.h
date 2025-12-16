#pragma once

#include "BlueprintUtilities/BlueprintFunctionLibrary/ClassUtilities.h"
#include "PropertyObjectEditor.generated.h"

UINTERFACE()
class COREGAMEUI_API UPropertyObjectEditor : public UInterface {
	GENERATED_BODY()
};

class COREGAMEUI_API IPropertyObjectEditor {
	GENERATED_BODY()

private:
	template<typename ValueType>
	ValueType* ReadValueFromPropertyInternal(FProperty* property) const {
		if (!property || !EditObject) return nullptr;

		ValueType* value = nullptr;
		void* object;
		if (ArrayProperty) {
			FScriptArrayHelper helper(ArrayProperty, EditObject);
			object = helper.GetRawPtr();
		}
		else object = EditObject;
		return property->ContainerPtrToValuePtr<ValueType>(object);
	}

	template<typename ValueType>
	bool WriteValueToPropertyInternal(FProperty* property, const ValueType& newValue) {
		if (!property || !EditObject) return false;

		if (ArrayProperty) {
			FScriptArrayHelper helper(ArrayProperty, EditObject);
			const int32 count = helper.Num();
			bool valueChanged = false;
			for (int32 index = 0; index < count; index++) {
				if (ValueType* value = property->ContainerPtrToValuePtr<ValueType>(helper.GetRawPtr(index))) {
					const ValueType previousValue = *value;
					*value = newValue;
					if (previousValue != newValue) valueChanged = true;
				}
			}
			return valueChanged;
		}

		if (ValueType* value = property->ContainerPtrToValuePtr<ValueType>(EditObject)) {
			const ValueType previousValue = *value;
			*value = newValue;
			return previousValue != newValue;
		}
		else return false;
	}

protected:
	template<typename PropertyType = FProperty>
	static void GetPropertiesForObjectWithType(UStruct* type, TArray<FProperty*>& properties) {
		UClassUtilities::GetAllPropertiesForType<PropertyType>(properties, type);
	}
	template<typename PropertyType = FProperty>
	static void GetPropertyNamesForObjectWithType(UStruct* type, TArray<FString>& names) {
		UClassUtilities::GetAllPropertyNamesForType<PropertyType>(names, type);
	}

	void* EditObject;
	FArrayProperty* ArrayProperty;
	FProperty* EditProperty;
	FProperty* ControlProperty;

	template<typename PropertyType = FProperty>
	FProperty* GetPropertyFromName(UStruct* type, const FString& propertyName) const {
		if (type) {
			TArray<FProperty*> properties;
			GetPropertiesForObjectWithType<PropertyType>(type, properties);
			for (FProperty* property : properties) {
				if (property->GetName().Equals(propertyName, ESearchCase::IgnoreCase))
					return property;
			}
		}

		return nullptr;
	}

	template<typename ValueType>
	ValueType GetPropertyValue() const {
		ValueType* value = ReadValueFromPropertyInternal<ValueType>(EditProperty);
		return value ? *value : ValueType();
	}
	template<typename ValueType>
	ValueType* GetPropertyValuePtr() const {
		return ReadValueFromPropertyInternal<ValueType>(EditProperty);
	}

	template<typename ValueType>
	bool SetPropertyValue(const ValueType& newValue) {
		return WriteValueToPropertyInternal(EditProperty, newValue);
	}

	template<typename ValueType = bool>
	ValueType GetControlValue() const {
		ValueType* value = ReadValueFromPropertyInternal<ValueType>(ControlProperty);
		return value ? *value : ValueType();
	}

	template<typename ValueType = bool>
	ValueType* GetControlValuePtr() const {
		return ReadValueFromPropertyInternal<ValueType>(ControlProperty);
	}

	template<typename ValueType = bool>
	bool SetControlValue(const ValueType& newValue) {
		return WriteValueToPropertyInternal(ControlProperty, newValue);
	}

public:
	void SetEditableObject(UStruct* type, void* obj, const FString& propertyName, const FString* controlPropertyName = nullptr) {
		SetEditableObject(obj, GetPropertyFromName(type, propertyName), controlPropertyName ? GetPropertyFromName(type, *controlPropertyName) : nullptr);
	}

	void SetEditableObject(void* obj, FProperty* property, FProperty* controlProperty = nullptr) {
		if (obj && !ensureAlwaysMsgf(property, TEXT("'property' can only be null when the object is null"))) return;

		EditObject = obj;
		EditProperty = property;
		ControlProperty = controlProperty;
	}

	UFUNCTION(Category = "Input Value", BlueprintCallable, BlueprintNativeEvent, CustomThunk, meta = (CustomStructureParam = "obj"))
	void SetControlledObject(const UObject* obj);

	void SetControlledObject_Implementation(const UObject* obj) {
		// should never be called as it uses the CustomThunk function defined below
		check(false);
	}

	virtual void SetObjectWithType(UStruct* obj, void* data) {
		PURE_VIRTUAL(__FUNCTION__);
	}

	DECLARE_FUNCTION(execSetControlledObject) {
		Stack.MostRecentProperty = nullptr;
		Stack.Step(Stack.Object, nullptr);

		FArrayProperty* arrayProperty = ((IPropertyObjectEditor*)Context)->ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
		FProperty* property = arrayProperty ? arrayProperty->Inner : Stack.MostRecentProperty;

		if (FStructProperty* structProperty = CastField<FStructProperty>(property)) {
			P_FINISH;
			P_NATIVE_BEGIN;
			((IPropertyObjectEditor*)Context)->SetObjectWithType(structProperty->Struct, Stack.MostRecentPropertyAddress);
			P_NATIVE_END;
		}
		else if (FObjectProperty* objectProperty = CastField<FObjectProperty>(property)) {
			P_FINISH;
			P_NATIVE_BEGIN;
			if (arrayProperty) {
				((IPropertyObjectEditor*)Context)->SetObjectWithType(objectProperty->GetOwnerClass(), Stack.MostRecentPropertyAddress);
			}
			else {
				UObject* obj = objectProperty->GetObjectPropertyValue(Stack.MostRecentPropertyAddress);
				((IPropertyObjectEditor*)Context)->SetObjectWithType(obj->GetClass(), obj);
			}
			P_NATIVE_END;
		}
		else Stack.bAbortingExecution = true;
	}
};
