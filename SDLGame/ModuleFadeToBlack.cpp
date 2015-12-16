#include "ModuleFadeToBlack.h"
#include "Application.h"
#include "ModuleRender.h"


ModuleFadeToBlack::ModuleFadeToBlack(bool started):Module(started) {}


ModuleFadeToBlack::~ModuleFadeToBlack() {}

bool ModuleFadeToBlack::start() {
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->renderer->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

update_status ModuleFadeToBlack::update() {
	if (startTime > 0) {
		Uint32 now = SDL_GetTicks() - startTime;
		float normalized = (float) now / (float) totalTime;

		if (normalized > 1.0f) {
			normalized = 1.0f;
		}

		if (fadingIn == false) {
			normalized = 1.0f - normalized;
		}

		// Draw a screen-size balck rectangle with alpha
		SDL_SetRenderDrawColor(App->renderer->renderer, 0, 0, 0, (Uint8) (normalized * 255.0f));
		SDL_RenderFillRect(App->renderer->renderer, nullptr);

		if (moduleOff == nullptr && moduleOn != nullptr) {
			moduleOn->enable();
			moduleOn = nullptr;
		}

		if (now >= totalTime) {
			if (fadingIn == true) {
				if (moduleOff != nullptr)
					moduleOff->disable();
				moduleOn->enable();

				totalTime += totalTime;
				startTime = SDL_GetTicks();
				fadingIn = false;
			} else {
				startTime = 0;
			}
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModuleFadeToBlack::postUpdate() {
	

	return UPDATE_CONTINUE;
}

// Fade to black. At mid point deactivate one module, then activate the other
void ModuleFadeToBlack::fadeToBlack(Module * moduleOn, Module * moduleOff, float time) {
	fadingIn = (moduleOff != nullptr) ? true : false;
	startTime = SDL_GetTicks();
	totalTime = (Uint32) (time  * 0.5f * 1000.0f);

	this->moduleOn = moduleOn;
	this->moduleOff = moduleOff;
}

bool ModuleFadeToBlack::isFading() const {
	return startTime>0;
}
