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
	TSubclassOf<UCoreMainMenuWidget> MainMenuWidgetClass;
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<UCoreMainMenuWidget> MainMenuWidget;

	UPROPERTY(Category = "Widget Types", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCoreGameWidget> MainGameWidgetClass;
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<UCoreGameWidget> MainGameWidget;

	UFUNCTION(BlueprintCallable)
	virtual inline bool IsMainMenuVisible() const;
	UFUNCTION(BlueprintCallable)
	virtual inline void ToggleMainMenuVisible() { SetMainMenuVisible(!IsMainMenuVisible()); }
	UFUNCTION(BlueprintCallable)
	virtual void SetMainMenuVisible(bool visible);
};
