#include "CoreGameWidget.h"

void UCoreGameWidget::Show() {
	AddToViewport(ZOrder);
}

void UCoreGameWidget::Hide() {
	RemoveFromParent();
	Destruct();
}
