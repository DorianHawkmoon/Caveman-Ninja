#pragma once
#ifndef GUI_LIFEBAR_H
#define GUI_LIFEBAR_H

#include "GUIComponent.h"
#include <vector>
#include <string>
class Entity;
class LifeComponent;

namespace GUI {
	enum PointColor {
		GREEN = 0,
		YELLOW,
		RED,
		EMPTY,
		COUNT_COLOR
	};

	class GUILifeBar : public GUIComponent {
	public:
		GUILifeBar(const std::string& nameTexture, const LifeComponent* life);
		virtual ~GUILifeBar();

		bool start();
		bool cleanUp();
		update_status update();
		void draw(const GUITransform& states) const;
		
		inline bool isSelectable() const {
			return false;
		}
		virtual void handleEvent(const SDL_Event& event) {}

		std::vector<SDL_Rect> pointColor;
		unsigned int longLife;

	private:
		const std::string nameTexture;
		SDL_Texture* texture;
		const LifeComponent* life;

		int coloredPoint;
		PointColor actualColor;

		fPoint getPosition(const GUITransform& transformParent) const;
	};
}
#endif // !GUI_LIFEBAR_H

