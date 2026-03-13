#pragma once

#include "Components/Button.h"
#include "LabelButton.generated.h"

UCLASS()
class COREGAMEUI_API ULabelButton : public UButton {
	GENERATED_BODY()

protected:
	TSharedPtr<class STextBlock> ButtonLabel;

	virtual TSharedRef<class SWidget> RebuildWidget() override;

	UPROPERTY(Category = "Behavior", EditAnywhere)
	FText ButtonText;

public:
	static const struct FButtonStyle DefaultButtonStyle;

	ULabelButton(const FObjectInitializer& initializer = FObjectInitializer::Get());
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UFUNCTION(Category = "Behavior", BlueprintCallable)
	FText GetButtonText() const { return ButtonText; }

	UFUNCTION(Category = "Behavior", BlueprintCallable)
	void SetButtonText(const FText& label);
};
