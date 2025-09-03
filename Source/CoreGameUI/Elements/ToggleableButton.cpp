#include "ToggleableButton.h"

void SToggleableButton::SetSelected(const bool selected) {
	if (selected != IsSelected()) {
		isSelected = selected;
		SetAppearPressed(isSelected);
	}
}
