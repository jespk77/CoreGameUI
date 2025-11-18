#pragma once

#include "InputWidgets.h"
#include "PropertyWidgets.generated.h"

UINTERFACE()
class COREGAMEUI_API UPropertyObjectEditor : public UInterface {
	GENERATED_BODY()
};

class COREGAMEUI_API IPropertyObjectEditor {
	GENERATED_BODY()

private:
	template<typename ValueType>
	ValueType ReadValueFromPropertyInternal(FProperty* property) const;
	template<typename ValueType>
	bool WriteValueToPropertyInternal(FProperty* property, const ValueType& newValue);

protected:
	template<typename PropertyType = FProperty>
	static void GetPropertiesForObjectWithType(UStruct* type, TArray<FProperty*>& properties);
	template<typename PropertyType = FProperty>
	static void GetPropertyNamesForObjectWithType(UStruct* type, TArray<FString>& names);

	void* EditObject;
	FArrayProperty* ArrayProperty;
	FProperty* EditProperty;
	FProperty* ControlProperty;

	template<typename PropertyType = FProperty>
	FProperty* GetPropertyFromName(UStruct* type, const FString& propertyName) const;

	template<typename ValueType>
	ValueType GetPropertyValue() const;

	template<typename ValueType>
	bool SetPropertyValue(const ValueType& newValue);

	template<typename ValueType = bool>
	ValueType GetControlValue() const;

	template<typename ValueType = bool>
	bool SetControlValue(const ValueType& newValue);

public:
	void SetEditableObject(UStruct* type, void* obj, const FString& propertyName, const FString* controlPropertyName = nullptr);
	void SetEditableObject(void* obj, FProperty* property, FProperty* controlProperty = nullptr);

	UFUNCTION(Category = "Input Value", BlueprintCallable, BlueprintNativeEvent, CustomThunk, meta = (CustomStructureParam = "obj"))
	void SetControlledObject(const UObject* obj);
	void SetControlledObject_Implementation(const UObject* obj) {
		// should never be called as it uses the CustomThunk function defined below
		check(false);
	}

	virtual void SetObjectWithType(UStruct* obj, void* data) { }

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

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class COREGAMEUI_API UBooleanPropertyWidget : public UBooleanInputWidget, public IPropertyObjectEditor {
	GENERATED_BODY()

private:
	UFUNCTION() TArray<FString> GetPropertiesForObject() const;

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "PropertyStruct == nullptr", EditConditionHides))
	UClass* PropertyClass;
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "PropertyClass == nullptr", EditConditionHides))
	UScriptStruct* PropertyStruct;
#endif

	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly, meta = (GetOptions = GetPropertiesForObject))
	FString PropertyName;

public:
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadWrite)
	bool UpdateWidgetOnTick = false;

	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& geometry, float delta) override;

	virtual bool GetValue() const override { return GetPropertyValue<bool>(); }
	virtual bool SetValue(const bool newValue) override;
	virtual void SetObjectWithType(UStruct* obj, void* data) override;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class COREGAMEUI_API UNumericPropertyWidget : public UNumericInputWidget, public IPropertyObjectEditor {
	GENERATED_BODY()

private:
	UFUNCTION() TArray<FString> GetPropertiesForObject() const;

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "PropertyStruct == nullptr", EditConditionHides))
	UClass* PropertyClass;
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "PropertyClass == nullptr", EditConditionHides))
	UScriptStruct* PropertyStruct;
#endif

	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly, meta = (GetOptions = GetPropertiesForObject))
	FString PropertyName;

public:
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadWrite)
	bool UpdateWidgetOnTick = false;

	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& geometry, float delta) override;

	virtual float GetValue() const override { return GetPropertyValue<float>(); }
	virtual float SetValue(const float newValue) override;
	virtual void SetObjectWithType(UStruct* obj, void* data) override;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class COREGAMEUI_API UToggleNumericPropertyWidget : public UToggleNumericInputWidget, public IPropertyObjectEditor {
	GENERATED_BODY()

private:
	UFUNCTION() TArray<FString> GetPropertiesForObject() const;
	UFUNCTION() TArray<FString> GetConditionalPropertiesForObject() const;

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "PropertyStruct == nullptr", EditConditionHides))
	UClass* PropertyClass;
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "PropertyClass == nullptr", EditConditionHides))
	UScriptStruct* PropertyStruct;
#endif

	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly, meta = (GetOptions = GetPropertiesForObject))
	FString PropertyName;
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly, meta = (GetOptions = GetConditionalPropertiesForObject))
	FString EditConditionPropertyName;

public:
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadWrite)
	bool UpdateWidgetOnTick = false;

	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& geometry, float delta) override;

	virtual float GetValue() const override { return GetPropertyValue<float>(); }
	virtual float SetValue(const float newValue) override;
	virtual bool GetEnabled() const override { return GetControlValue(); }
	virtual bool SetEnabled(bool enabled) override;
	virtual void SetObjectWithType(UStruct* obj, void* data) override;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class COREGAMEUI_API USelectionPropertyWidget : public USelectionInputWidget, public IPropertyObjectEditor {
	GENERATED_BODY()

private:
	UFUNCTION() TArray<FString> GetPropertiesForObject() const;

protected:
	UEnum* EnumProperty;
	virtual void UpdateEnumProperty();

#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "PropertyStruct == nullptr", EditConditionHides))
	UClass* PropertyClass;
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "PropertyClass == nullptr", EditConditionHides))
	UScriptStruct* PropertyStruct;
#endif

	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly, meta = (GetOptions = GetPropertiesForObject))
	FString PropertyName;

public:
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadWrite)
	bool UpdateWidgetOnTick = false;

	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& geometry, float delta) override;

	virtual uint8 GetValue_uint8() const { return GetPropertyValue<uint8>(); }
	virtual int32 GetValue() const override { return GetValue_uint8(); }
	virtual int32 SetValue(const int32 newValue) override { return SetValue_uint8((uint8)FMath::Clamp(newValue, 0, 255)); }
	virtual uint8 SetValue_uint8(const uint8 newValue);
	virtual void SetObjectWithType(UStruct* obj, void* data) override;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class COREGAMEUI_API UButtonSelectionPropertyWidget : public USelectionPropertyWidget {
	GENERATED_BODY()

protected:
	TArray<TSharedPtr<SToggleableButton>> Buttons;

	virtual void UpdateEnumProperty() override;
	virtual void UpdateWidget() override;

	virtual void AddCustomElements() override { }
	virtual void ReleaseCustomElements() override;
};
