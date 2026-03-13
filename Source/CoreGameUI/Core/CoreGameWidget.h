#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreGameWidget.generated.h"

UCLASS(Abstract)
class COREGAMEUI_API UCoreGameWidget : public UUserWidget {
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Widget Properties", EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0, Delta = 1))
	int32 ZOrder = 1;

	template<typename OwnerType, typename WidgetClass>
	static WidgetClass* ShowWidget(OwnerType* owner, TSubclassOf<WidgetClass> widgetClass = WidgetClass::StaticClass(), const FName widgetName = NAME_None) {
		static_assert(!TIsAbstract<UCoreGameWidget>::Value, "Widget class cannot be Abstract");
		static_assert(TIsDerivedFrom<WidgetClass, UCoreGameWidget>::IsDerived, "Widget class must be a CoreGameWidget");
		WidgetClass* widget = CreateWidget<WidgetClass>(owner, widgetClass, widgetName);
		if (ensure(widget)) widget->Show();
		return widget;
	}

	UFUNCTION(Category = "Widget Control", BlueprintCallable, meta = (DeterminesOutputType = "widgetClass"))
	static UCoreGameWidget* ShowWidget(APlayerController* owner, TSubclassOf<UCoreGameWidget> widgetClass, const FName widgetName = NAME_None) {
		return ShowWidget<APlayerController, UCoreGameWidget>(owner, widgetClass, widgetName);
	}

	UFUNCTION(Category = "Widget Control", BlueprintCallable)
	virtual void Show();

	UFUNCTION(Category = "Widget Control", BlueprintCallable)
	virtual void Hide();
};
