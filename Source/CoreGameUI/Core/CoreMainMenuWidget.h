#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMainMenuWidget.generated.h"

UCLASS(Abstract)
class COREGAMEUI_API UCoreMainMenuWidget : public UUserWidget {
	GENERATED_BODY()

protected:
	UFUNCTION()
	virtual void OnVisibilityChange(const ESlateVisibility visible);

	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION(Category = "Common Controls", BlueprintCallable)
	virtual bool IsOpen() const { return IsVisible(); }
	UFUNCTION(Category = "Common Controls", BlueprintCallable)
	virtual void OpenMenu();
	UFUNCTION(Category = "Common Controls", BlueprintCallable)
	virtual void CloseMenu();

	UFUNCTION(Category = "Common Controls", BlueprintCallable)
	virtual void ExitGame(bool force = false);
};
