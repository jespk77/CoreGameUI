#pragma once

#include "Widgets/Input/SButton.h"

class COREGAMEUI_API SToggleableButton : public SButton {
	//SLATE_DECLARE_WIDGET_API(SToggleableButton, SButton, COREGAMEUI_API);
	typedef SButton Super;

protected:
	bool isSelected = false;

public:
	bool IsSelected() const { return isSelected; }
	void SetSelected(const bool selected);
};
