#pragma once

#include "InputWidgets.h"
#include "PropertyWidgets.generated.h"

class IPropertyObjectEditor {
protected:
	template<typename PropertyType>
	static TArray<FProperty*> GetPropertiesForObjectWithType(UClass* class_);
	template<typename PropertyType>
	static TArray<FString> GetPropertyNamesForObjectWithType(UClass* class_);

	UObject* PropertyObject;
	FProperty* EditProperty;

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
	virtual void NativePreConstruct() override;
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
	virtual void NativePreConstruct() override;
	virtual float GetValue() const override { return GetPropertyValue<float>(); }
	virtual float SetValue(const float newValue) override;

	UFUNCTION(Category = "Input Value", BlueprintCallable)
	void SetObject(UObject* obj);
};
