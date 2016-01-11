#pragma once
#ifndef GUI_LABEL_H
#define GUI_LABEL_H

#include <string>
#include "GUITexture.h"
#include "SDL\SDL.h"
#include "SDL_ttf\SDL_ttf.h"

namespace GUI {
	/**
	* Etiqueta GUI
	*/
	class GUILabel : public GUITexture {
	public:
		/**
		* Constructor
		* @param text texto de la etiqueta
		* @param fonts fuentes que puede usar
		*/
		GUILabel(const std::string& text, const SDL_Color& color, const std::string& font, const GUILocation& location, int size = 12);
		/**
		* Destructor
		*/
		virtual ~GUILabel();

		virtual bool isSelectable() const;
	
		virtual void handleEvent(const SDL_Event& event);

		SDL_Color getColor() {
			return color;
		}


		void setColor(const SDL_Color& color) {
			this->color = color;
			createTexture();
		}

		std::string getText() {
			return text;
		}

		void setText(std::string& text) {
			this->text = text;
			createTexture();
		}

		void setSize(unsigned int size);

		inline unsigned int getSize() {
			return size;
		}

	private:
		std::string nameFont;
		unsigned int size;
		TTF_Font* font;

		/**
		* Texto
		*/
		std::string text;
		SDL_Color color;

		void createTexture();
	};
}
#endif // !GUI_LABEL_H
