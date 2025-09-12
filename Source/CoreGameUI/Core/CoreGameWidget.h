#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreGameWidget.generated.h"

UCLASS(Abstract)
class COREGAMEUI_API UCoreGameWidget : public UUserWidget {
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Widget Properties", EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 ZOrder = 1;
};
