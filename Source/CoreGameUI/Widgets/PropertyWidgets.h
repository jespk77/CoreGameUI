#pragma once

#include "InputWidgets.h"
#include "PropertyObjectEditor.h"
#include "PropertyWidgets.generated.h"

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
	virtual bool GetEnabled() const override { return !IsReadOnly ? GetControlValue() : false; }
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

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class COREGAMEUI_API UStringPropertyWidgetWidget : public UStringInputWidget, public IPropertyObjectEditor {
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

	virtual FString GetValue() const override { return GetPropertyValue<FString>(); }
	virtual FString SetValue(const FString& newValue) override;
	virtual void SetObjectWithType(UStruct* obj, void* data) override;
};
