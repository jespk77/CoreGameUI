#include "PropertyWidgetContainer.h"
#include "PropertyWidgets.h"
#include "Blueprint/WidgetTree.h"
#include "BlueprintUtilities/BlueprintFunctionLibrary/ClassUtilities.h"

void IPropertyWidgetContainer::SetObjectOnPropertyWidgets(UStruct* type, void* data) {
	UUserWidget* parent = Cast<UUserWidget>(_getUObject());
	if (!ensureMsgf(parent, TEXT("Objects with the IPropertyWidgetContainer interface should derive from UUserWidget"))) return;

	TArray<UWidget*> widgets;
	parent->WidgetTree->GetAllWidgets(widgets);
	for (UWidget* widget : widgets) {
		if (IPropertyObjectEditor* editor = Cast<IPropertyObjectEditor>(widget))
			editor->SetObjectWithType(type, data);
	}
}

void IPropertyWidgetContainer::SetObjectOnPropertyWidgets_Implementation(UObject* object) {
	SetObjectOnPropertyWidgets(object ? object->GetClass() : nullptr, object);
}

void IPropertyWidgetContainer::SetObjectsOnPropertyWidgets_Implementation(const TArray<UObject*>& objects) {
	SetObjectOnPropertyWidgets(UClassUtilities::GetCommonClassFromArray(objects), objects.GetData()[0]);
}
