#pragma once

#include "PropertyWidgetContainer.generated.h"

UINTERFACE()
class COREGAMEUI_API UPropertyWidgetContainer : public UInterface {
	GENERATED_BODY()
};

class COREGAMEUI_API IPropertyWidgetContainer {
	GENERATED_BODY()

public:
	void SetObjectOnPropertyWidgets(UStruct* type, void* data);

	UFUNCTION(Category = "Property Widget", BlueprintCallable, BlueprintNativeEvent)
	void SetObjectOnPropertyWidgets(UObject* object);
	virtual void SetObjectOnPropertyWidgets_Implementation(UObject* object);

	UFUNCTION(Category = "Property Widget", BlueprintCallable, BlueprintNativeEvent)
	void SetObjectsOnPropertyWidgets(const TArray<UObject*>& objects);
	virtual void SetObjectsOnPropertyWidgets_Implementation(const TArray<UObject*>& objects);
};
