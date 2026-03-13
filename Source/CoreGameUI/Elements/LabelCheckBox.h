#pragma once

#include "Components/CheckBox.h"
#include "LabelCheckBox.generated.h"

UCLASS()
class COREGAMEUI_API ULabelCheckBox : public UCheckBox {
	GENERATED_BODY()

protected:
	TSharedPtr<class SHorizontalBox> Container;
	TSharedPtr<class STextBlock> CheckboxLabel;

	virtual TSharedRef<class SWidget> RebuildWidget() override;

	UPROPERTY(Category = "Behavior", EditAnywhere)
	FText CheckboxText;

public:
	static const struct FCheckBoxStyle DefaultCheckboxStyle;

	ULabelCheckBox(const FObjectInitializer& initializer = FObjectInitializer::Get());
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UFUNCTION(Category = "Behavior", BlueprintCallable)
	FText GetCheckboxText() const { return CheckboxText; }

	UFUNCTION(Category = "Behavior", BlueprintCallable)
	void SetCheckboxText(const FText& label);
};
