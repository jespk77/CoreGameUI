#pragma once

#include "InputWidgets.h"
#include "PropertyWidgets.generated.h"

class IPropertyObjectEditor {
protected:
	template<typename PropertyType = FProperty>
	static void GetPropertiesForObjectWithType(UClass* class_, TArray<FProperty*>& properties);
	template<typename PropertyType = FProperty>
	static void GetPropertyNamesForObjectWithType(UClass* class_, TArray<FString>& names);

	UObject* PropertyObject;
	FProperty* EditProperty;

	template<typename PropertyType = FProperty>
	void SetPropertyFromName(UClass* class_, const FString& propertyName);
	void SetEditableObject(UObject* obj, const FString& propertyName);

	template<typename ValueType>
	ValueType GetPropertyValue() const;

	template<typename ValueType>
	bool SetPropertyValue(const ValueType& newValue);
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class COREGAMEUI_API UBooleanPropertyWidget : public UBooleanInputWidget, public IPropertyObjectEditor {
	GENERATED_BODY()

private:
	UFUNCTION()
	TArray<FString> GetPropertiesForObject() const;

protected:
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	UClass* PropertyClass;
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly, meta = (GetOptions = GetPropertiesForObject))
	FString PropertyName;

public:
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadWrite)
	bool UpdateWidgetOnTick = false;

	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& geometry, float delta) override;

	virtual bool GetValue() const override { return GetPropertyValue<bool>(); }
	virtual bool SetValue(const bool newValue) override;

	UFUNCTION(Category = "Input Value", BlueprintCallable)
	void SetObject(UObject* obj);
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class COREGAMEUI_API UNumericPropertyWidget : public UNumericInputWidget, public IPropertyObjectEditor {
	GENERATED_BODY()

private:
	UFUNCTION()
	TArray<FString> GetPropertiesForObject() const;

protected:
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	UClass* PropertyClass;
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly, meta = (GetOptions = GetPropertiesForObject))
	FString PropertyName;

public:
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadWrite)
	bool UpdateWidgetOnTick = false;

	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& geometry, float delta) override;

	virtual float GetValue() const override { return GetPropertyValue<float>(); }
	virtual float SetValue(const float newValue) override;

	UFUNCTION(Category = "Input Value", BlueprintCallable)
	void SetObject(UObject* obj);
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class COREGAMEUI_API USelectionPropertyWidget : public USelectionInputWidget, public IPropertyObjectEditor {
	GENERATED_BODY()

protected:
	UEnum* EnumProperty;
	virtual void UpdateEnumProperty();

	UFUNCTION()
	TArray<FString> GetPropertiesForObject() const;

	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	UClass* PropertyClass;
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

	UFUNCTION(Category = "Input Value", BlueprintCallable)
	void SetObject(UObject* obj);
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class COREGAMEUI_API UButtonSelectionPropertyWidget : public USelectionPropertyWidget {
	GENERATED_BODY()

protected:
	TArray<TSharedPtr<SButton>> Buttons;

	virtual void UpdateEnumProperty() override;
	virtual void UpdateWidget() override;

	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	FButtonStyle UnselectedStyle;
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	FButtonStyle SelectedStyle;

	virtual void AddCustomElements() override;
	virtual void ReleaseCustomElements() override;
};
