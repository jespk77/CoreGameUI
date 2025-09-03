#pragma once

#include "InputWidgets.h"
#include "PropertyWidgets.generated.h"

UINTERFACE()
class COREGAMEUI_API UPropertyObjectEditor : public UInterface {
	GENERATED_BODY()
};

class COREGAMEUI_API IPropertyObjectEditor {
	GENERATED_BODY()

protected:
	template<typename PropertyType = FProperty>
	static void GetPropertiesForObjectWithType(UClass* class_, TArray<FProperty*>& properties);
	template<typename PropertyType = FProperty>
	static void GetPropertyNamesForObjectWithType(UClass* class_, TArray<FString>& names);

	UObject* PropertyObject;
	FProperty* EditProperty;

	template<typename PropertyType = FProperty>
	FProperty* GetPropertyFromName(UClass* class_, const FString& propertyName) const;

	template<typename ValueType>
	ValueType GetPropertyValue() const;

	template<typename ValueType>
	bool SetPropertyValue(const ValueType& newValue);

public:
	void SetEditableObject(UObject* obj, const FString& propertyName);
	void SetEditableObject(UObject* obj, FProperty* property);

	UFUNCTION(Category = "Input Value", BlueprintNativeEvent)
	void SetObject(UObject* obj);
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class COREGAMEUI_API UBooleanPropertyWidget : public UBooleanInputWidget, public IPropertyObjectEditor {
	GENERATED_BODY()

private:
#if WITH_EDITORONLY_DATA
	UFUNCTION() TArray<FString> GetPropertiesForObject() const;
#endif

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	UClass* PropertyClass;
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
	virtual void SetObject_Implementation(UObject* obj) override;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class COREGAMEUI_API UNumericPropertyWidget : public UNumericInputWidget, public IPropertyObjectEditor {
	GENERATED_BODY()

private:
#if WITH_EDITORONLY_DATA
	UFUNCTION() TArray<FString> GetPropertiesForObject() const;
#endif

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	UClass* PropertyClass;
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
	virtual void SetObject_Implementation(UObject* obj) override;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class COREGAMEUI_API USelectionPropertyWidget : public USelectionInputWidget, public IPropertyObjectEditor {
	GENERATED_BODY()

private:
#if WITH_EDITORONLY_DATA
	UFUNCTION() TArray<FString> GetPropertiesForObject() const;
#endif

protected:
	UEnum* EnumProperty;
	virtual void UpdateEnumProperty();

#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	UClass* PropertyClass;
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
	virtual void SetObject_Implementation(UObject* obj) override;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class COREGAMEUI_API UButtonSelectionPropertyWidget : public USelectionPropertyWidget {
	GENERATED_BODY()

protected:
	TArray<TSharedPtr<SToggleableButton>> Buttons;

	virtual void UpdateEnumProperty() override;
	virtual void UpdateWidget() override;

	virtual void AddCustomElements() override;
	virtual void ReleaseCustomElements() override;
};
