#include "GUIComponent.h"


namespace GUI {

	GUIComponent::GUIComponent() : selected(false), actived(false) {
		transform.setToZero();
		transform.location = ABSOLUTE;
	}

	GUIComponent::~GUIComponent() {}

	bool GUIComponent::isSelected() const {
		return selected;
	}

	void GUIComponent::select() {
		selected = true;
	}

	void GUIComponent::deselect() {
		selected = false;
	}

	bool GUIComponent::isActive() const {
		return actived;
	}

	void GUIComponent::activate() {
		actived = true;
	}

	void GUIComponent::deactivate() {
		actived = false;
	}
}