#pragma once

#include "Blueprint/UserWidget.h"
#include "InputWidgets.generated.h"

UCLASS(Abstract, NotBlueprintable)
class COREGAMEUI_API UInputWidgetBase : public UUserWidget {
	GENERATED_BODY()

protected:
	TSharedPtr<SHorizontalBox> Container;
	TSharedPtr<STextBlock> Label;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool releaseChildren) override;

	virtual void NativePreConstruct() override;
	virtual void AddCustomElements() { }
	virtual void ReleaseCustomElements() { }
	virtual void UpdateWidget();

public:
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	FText DisplayName;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBooleanValueUpdated, const bool, Value);

UCLASS(ClassGroup = "Input Widgets")
class COREGAMEUI_API UBooleanInputWidget : public UInputWidgetBase {
	GENERATED_BODY()

protected:
	TSharedPtr<SCheckBox> Checkbox;

	virtual void UpdateWidget() override;

	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	bool Value = false;

	virtual void AddCustomElements() override;
	virtual void ReleaseCustomElements() override;

public:
	UFUNCTION(Category = "Input Value", BlueprintCallable)
	virtual bool GetValue() const { return Value; }
	UFUNCTION(Category = "Input Value", BlueprintCallable)
	virtual bool SetValue(const bool newValue);
	UPROPERTY(Category = "Events", BlueprintAssignable)
	FBooleanValueUpdated OnValueUpdated;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNumericValueUpdated, const float, Value);

class SSlider;

UCLASS(ClassGroup = "Input Widgets")
class COREGAMEUI_API UNumericInputWidget : public UInputWidgetBase {
	GENERATED_BODY()

protected:
	TSharedPtr<SSlider> Slider;
	TSharedPtr<SEditableText> Text;

	virtual void UpdateWidget() override;

	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	float Value = 0.f;
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	float Minimum = 0.f;
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	float Maximum = 1.f;
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	float Step = 0.1f;

	virtual void AddCustomElements() override;
	virtual void ReleaseCustomElements() override;

public:
	UFUNCTION(Category = "Input Value", BlueprintCallable)
	virtual float GetValue() const { return Value; }
	UFUNCTION(Category = "Input Value", BlueprintCallable)
	virtual float SetValue(const float newValue);

	UFUNCTION(Category = "Input Value", BlueprintCallable)
	float GetMinimum() const { return Minimum; }
	UFUNCTION(Category = "Input Value", BlueprintCallable)
	float SetMinimum(const float min);

	UFUNCTION(Category = "Input Value", BlueprintCallable)
	float GetMaximum() const { return Maximum; }
	UFUNCTION(Category = "Input Value", BlueprintCallable)
	float SetMaximum(const float max);

	UFUNCTION(Category = "Input Value", BlueprintCallable)
	float GetStep() const { return Step; }
	UFUNCTION(Category = "Input Value", BlueprintCallable)
	float SetStep(const float newStep);

	UPROPERTY(Category = "Events", BlueprintAssignable)
	FNumericValueUpdated OnValueUpdated;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS(ClassGroup = "Input Widgets")
class COREGAMEUI_API UToggleNumericInputWidget : public UNumericInputWidget {
	GENERATED_BODY()

protected:
	TSharedPtr<SCheckBox> EditCheckbox;

	virtual void UpdateWidget() override;

	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	bool Enabled = true;

	virtual void AddCustomElements() override;
	virtual void ReleaseCustomElements() override;

public:
	UFUNCTION(Category = "Input Value", BlueprintCallable)
	bool GetEnabled() const { return Enabled; }
	UFUNCTION(Category = "Input Value", BlueprintCallable)
	bool SetEnabled(const bool isEnabled);

	UPROPERTY(Category = "Events", BlueprintAssignable)
	FBooleanValueUpdated OnEnabledUpdated;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIndexUpdated, const int32, Value);

UCLASS(ClassGroup = "Input Widgets")
class COREGAMEUI_API USelectionInputWidget : public UInputWidgetBase {
	GENERATED_BODY()

protected:
	TSharedPtr<STextBlock> Text;

	virtual void UpdateWidget() override;

	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	TArray<FText> Entries;
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	int32 Value = 0;

	virtual void AddCustomElements() override;
	virtual void ReleaseCustomElements() override;

public:
	UFUNCTION(Category = "Input Value", BlueprintCallable)
	virtual int32 GetValue() const { return Value; }
	UFUNCTION(Category = "Input Value", BlueprintCallable)
	virtual int32 SetValue(const int32 newValue);
	UFUNCTION(Category = "Input Value", BlueprintCallable)
	const FText& GetSelectedEntry() const { return Entries.IsValidIndex(GetValue()) ? Entries[GetValue()] : FText::GetEmpty(); }

	UPROPERTY(Category = "Events", BlueprintAssignable)
	FIndexUpdated OnValueUpdated;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS(ClassGroup = "Input Widgets")
class COREGAMEUI_API UButtonSelectionInputWidget : public USelectionInputWidget {
	GENERATED_BODY()

protected:
	TArray<TSharedPtr<SButton>> Buttons;

	virtual void UpdateWidget() override;

	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	FButtonStyle UnselectedStyle;
	UPROPERTY(Category = "Input Value", EditAnywhere, BlueprintReadOnly)
	FButtonStyle SelectedStyle;

	virtual void AddCustomElements() override;
	virtual void ReleaseCustomElements() override;
};
