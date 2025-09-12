#pragma once

#include "GameFramework/HUD.h"
#include "CoreGameHUD.generated.h"

class UCoreMainMenuWidget;
class UCoreGameWidget;

UCLASS()
class COREGAMEUI_API ACoreGameHUD : public AHUD {
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(Category = "Widget Types", EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UCoreMainMenuWidget> MainMenuWidgetClass;
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<UCoreMainMenuWidget> MainMenuWidget;

	UPROPERTY(Category = "Widget Types", EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UCoreGameWidget>> MainGameWidgetClasses;
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TArray<TObjectPtr<UCoreGameWidget>> MainGameWidgets;

	UFUNCTION(BlueprintCallable)
	virtual inline bool IsMainMenuVisible() const;
	UFUNCTION(BlueprintCallable)
	virtual inline void ToggleMainMenuVisible() { SetMainMenuVisible(!IsMainMenuVisible()); }
	UFUNCTION(BlueprintCallable)
	virtual void SetMainMenuVisible(bool visible);
};
