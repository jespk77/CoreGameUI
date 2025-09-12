#include "WorldObjectWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#define LOCATION_DEBUG 0

#if WITH_EDITOR && LOCATION_DEBUG
#include "Components/LineBatchComponent.h"
#endif

void UWorldObjectWidget::UpdateWidgetTransform_Implementation(const FVector2D& screenPosition) {
	SetRenderTranslation(screenPosition);
}

void UWorldObjectWidget::NativeConstruct() {
	Super::NativeConstruct();
	Controller = GetOwningPlayer();
	SetRenderTransformPivot(Pivot);
}

void UWorldObjectWidget::NativeDestruct() {
	Super::NativeDestruct();
	Controller = nullptr;
}

void UWorldObjectWidget::NativeTick(const FGeometry& geometry, float delta) {
	Super::NativeTick(geometry, delta);

	if (Controller && Controller->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation, IsPlayerViewportRelative)){
		ScreenLocation /= UWidgetLayoutLibrary::GetViewportScale(this);
		UpdateWidgetTransform(ScreenLocation);
	}

#if WITH_EDITOR && LOCATION_DEBUG
	ULineBatchComponent* batcher = GetWorld()->GetLineBatcher(UWorld::ELineBatcherType::World);
	batcher->DrawPoint(WorldLocation, FColor::Magenta, 20.f, 1, 1.f);
#endif
}

#undef LOCATION_DEBUG
