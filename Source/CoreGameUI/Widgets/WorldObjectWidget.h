#pragma once

#include "Blueprint/UserWidget.h"
#include "WorldObjectWidget.generated.h"

/* A widget that can be placed in a position in the world and automatically moves as the camera moves from that position */
UCLASS(Abstract)
class COREGAMEUI_API UWorldObjectWidget : public UUserWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<APlayerController> Controller;

	UFUNCTION(Category = "World Object", BlueprintNativeEvent)
	void UpdateWidgetTransform(const FVector2D& screenPosition);
	virtual void UpdateWidgetTransform_Implementation(const FVector2D& screenPosition);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& geometry, float delta) override;

public:
	UPROPERTY(Category = "World Object", EditAnywhere, BlueprintReadWrite)
	FVector2D Pivot = FVector2D::ZeroVector;
	UPROPERTY(Category = "World Object", VisibleInstanceOnly, BlueprintReadOnly)
	FVector WorldLocation = FVector::ZeroVector;
	UPROPERTY(Category = "World Object", VisibleInstanceOnly, BlueprintReadOnly)
	FVector2D ScreenLocation = FVector2D::ZeroVector;
	UPROPERTY(Category = "World Object", EditAnywhere, BlueprintReadWrite)
	bool IsPlayerViewportRelative = true;
};
