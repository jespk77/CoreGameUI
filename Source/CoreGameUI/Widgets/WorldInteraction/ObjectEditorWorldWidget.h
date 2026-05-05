#pragma once

#include "WorldObjectWidget.h"
#include "ObjectEditorWorldWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemWidgetClicked, UObjectEditorWorldWidgetItemBase*, ButtonWidget);

UCLASS(Abstract)
class COREGAMEUI_API UObjectEditorWorldWidgetItemBase : public UWorldObjectWidget {
	GENERATED_BODY()

protected:
	virtual FReply NativeOnMouseButtonUp(const FGeometry& geometry, const FPointerEvent& event) override;

public:
	UPROPERTY(Category = "Events", BlueprintAssignable)
	FOnItemWidgetClicked OnClicked;

	UFUNCTION(Category = "Widgets", BlueprintCallable)
	virtual void UpdateWorldLocation() { }
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS(Abstract)
class COREGAMEUI_API UObjectEditorWorldWidgetBase : public UUserWidget {
	GENERATED_BODY()

protected:
	virtual void NativeDestruct() override;

	UPROPERTY(Category = "References", EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UObjectEditorWorldWidgetItemBase> WidgetClass;
	UPROPERTY(Category = "References", EditAnywhere, BlueprintReadOnly)
	TArray<UObjectEditorWorldWidgetItemBase*> ConnectionWidgets;

	UFUNCTION(Category = "Widgets", BlueprintNativeEvent)
	void OnWorldButtonPressed(UObjectEditorWorldWidgetItemBase* widget);
	virtual void OnWorldButtonPressed_Implementation(UObjectEditorWorldWidgetItemBase* widget) { }

	virtual void FinishWidgetCreation(UObjectEditorWorldWidgetItemBase* widget);

	UFUNCTION(Category = "Widgets", BlueprintCallable)
	UObjectEditorWorldWidgetItemBase* CreateNewWidget() {
		return CreateNewWidget<UObjectEditorWorldWidgetItemBase>();
	}

	template<typename WidgetType = UObjectEditorWorldWidgetItemBase>
	FORCEINLINE WidgetType* CreateNewWidget() {
		WidgetType* widget = CreateWidget<WidgetType>(this, WidgetClass);
		check(widget);
		FinishWidgetCreation(widget);
		return widget;
	}

	UFUNCTION(Category = "Widgets", BlueprintCallable)
	virtual void DestroyCreatedWidgets();

public:
	UFUNCTION(Category = "Widgets", BlueprintCallable)
	void UpdateWidgetLocations();
};
