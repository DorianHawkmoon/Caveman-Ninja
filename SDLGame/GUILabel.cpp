#include "GUILabel.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"

#include <iostream>
#include <sstream>  // Required for stringstreams
#include <string> 

namespace GUI {

	GUILabel::GUILabel(const std::string& text, const SDL_Color& color, const std::string& font, const GUILocation& location, int size) 
								: GUI::GUITexture(), nameFont(font), text(text), size(size+1), color(color)  {
		transform.location = location;
		setSize(size);

	}

	bool GUILabel::isSelectable() const {
		return false;
	}

	void GUILabel::handleEvent(const SDL_Event&) {}

	SDL_Color GUILabel::getColor() const {
		return color;
	}

	void GUILabel::setColor(const SDL_Color & color) {
		this->color = color;
		createTexture();
	}

	std::string GUILabel::getText() const {
		return text;
	}

	void GUILabel::setText(const std::string & text) {
		this->text = text;
		createTexture();
	}

	void GUILabel::setSize(unsigned int size) {
		if (this->size != size) {
			if (font != nullptr) {
				App->fonts->unload(font, size);
			}
			this->size = size;
			font = App->fonts->load(nameFont.c_str(), size);
			createTexture();
		}
	}

	unsigned int GUILabel::getSize() {
		return size;
	}

	void GUILabel::createTexture() {
		//Render text surface
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
		if (textSurface == nullptr) {
			LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		} else {
			//Create texture from surface pixels
			texture = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
			if (texture == nullptr) {
				LOG("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
			}

			//Get rid of old surface
			SDL_FreeSurface(textSurface);
		}

		// Establece el offset de acuerdo al alineamiento
		if (transform.location == GUILocation::ABSOLUTE) {
			return;
		}
	}

	GUILabel::~GUILabel() {
		if (font != nullptr) {
			App->fonts->unload(font, size);
		}
	}


}