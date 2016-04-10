#include "GUIContainer.h"

namespace GUI {

	GUIContainer::GUIContainer() : children(), selectedChild(-1) {

	}

	void GUIContainer::pack(GUIComponent* component) {
		component->setParent(this);
		children.push_back(component);
	}

	void GUIContainer::clearSelection() {
		if (hasSelection()) {
			children[selectedChild]->deselect();
		}
		selectedChild = -1;
	}

	bool GUIContainer::isSelectable() const {
		return false;
	}

	void GUIContainer::handleEvent(const SDL_Event& event) {
		//If we have selected a child then give it events
		if (hasSelection() && children[selectedChild]->isActive()) {
			children[selectedChild]->handleEvent(event);

		} else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_W || event.key.keysym.scancode == SDL_SCANCODE_UP) {
				selectPrevious();
			} else if (event.key.keysym.scancode == SDL_SCANCODE_S || event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
				selectNext();
			} else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN || event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				if (hasSelection())
					children[selectedChild]->activate();
			}
		}
	}

	void GUIContainer::draw(const GUITransform& states) const {
		GUITransform trans = transform + states;

		// Determina la posición teniendo en cuenta el padre y la posición que tiene
		trans.position = getPosition(states);

		for (const GUIComponent* child : children) {
			child->draw(trans);
		}
	}

	void GUIContainer::setSize(iPoint size) {
		this->size = size;
	}

	bool GUIContainer::start() {
		for (auto it = children.begin(); it != children.end(); ++it) {
			(*it)->start();
		}
		return true;
	}

	update_status GUIContainer::preUpdate() {
		for (auto it = children.begin(); it != children.end(); ++it) {
			(*it)->preUpdate();
		}
		return UPDATE_CONTINUE;
	}

	update_status GUIContainer::update() {
		for (auto it = children.begin(); it != children.end(); ++it) {
			(*it)->update();
		}
		return UPDATE_CONTINUE;
	}

	bool GUIContainer::cleanUp() {
		for (auto it = children.begin(); it != children.end(); ++it) {
			(*it)->cleanUp();
		}
		return true;
	}

	bool GUIContainer::hasSelection() const {
		return selectedChild >= 0;
	}

	void GUIContainer::select(unsigned int index) {
		if (children[index]->isSelectable()) {
			if (hasSelection())
				children[selectedChild]->deselect();

			children[index]->select();
			selectedChild = index;
		}
	}

	void GUIContainer::selectNext() {
		if (!hasSelection()) {
			if (children.size() > 0) {
				int index = 0;
				select(index);
			}
		} else {

			// Search next component that is selectable, wrap around if necessary
			int next = selectedChild;
			do {
				next = (next + 1) % children.size();
			} while (!children[next]->isSelectable());

			// Select that component
			select(next);
		}
	}

	void GUIContainer::selectPrevious() {
		if (!hasSelection()) {
			int index = (int)children.size() - 1;
			if (index >= 0) {
				select(index);
			}
		} else {

			// Search previous component that is selectable, wrap around if necessary
			int prev = selectedChild;
			do {
				prev = (prev + (int)children.size() - 1) % children.size();
			} while (!children[prev]->isSelectable());

			// Select that component
			select(prev);
		}
	}

	GUIContainer::~GUIContainer() {
		for (auto it = children.begin(); it != children.end(); ++it) {
			delete (*it);
		}
		children.clear();
	}


}