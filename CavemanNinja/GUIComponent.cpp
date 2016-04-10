#include "GUIComponent.h"
#include "Globals.h"


namespace GUI {

	GUIComponent::GUIComponent() : selected(false), actived(false), offset(0,0) {
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
	const iPoint GUIComponent::getSize() const {
		return size;
	}
	const GUIComponent * GUIComponent::getParent() const {
		return parent;
	}
	void GUIComponent::setParent(const GUIComponent * child) {
		parent = child;
	}

	fPoint GUIComponent::getPosition(const GUITransform& transformParent) const {
		fPoint position = transformParent.position;

		if (transform.location == ABSOLUTE) {
			position += transform.position;
		} else {
			iPoint sizeParent = getParent()->getSize();
			//start centered as default
			position += fPoint(sizeParent.x * 0.5f, sizeParent.y * 0.5f);

			if ((transform.location & TOP) > 0) {
				position -= fPoint(0.0f, sizeParent.y * 0.5f);
			}
			if ((transform.location & BOTTOM) > 0) {
				position += fPoint(0.0f, sizeParent.y * 0.5f);
			}
			if ((transform.location & LEFT) > 0) {
				position -= fPoint(sizeParent.x * 0.5f, 0.0f);
			}
			if ((transform.location & RIGHT) > 0) {
				position += fPoint(sizeParent.x * 0.5f, 0.0f);
			}
		}

		//offset is given in base units, instead of parent size, given in its final size
		position += offset*SCREEN_SIZE;

		return position;
	}
}