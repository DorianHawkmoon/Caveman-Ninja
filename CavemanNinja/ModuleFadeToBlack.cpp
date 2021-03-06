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

bool ModuleFadeToBlack::cleanUp() {
	//SDL_SetRenderDrawBlendMode(App->renderer->renderer, SDL_BLENDMODE_NONE);
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
		SDL_Color color;
		color.r = 0;
		color.g = 0;
		color.b = 0;
		App->renderer->setFaddingEffect(normalized, color);

		if (moduleOff == nullptr && moduleOn != nullptr) {
			moduleOn->enable();
			moduleOn = nullptr;
		}

		if (now >= totalTime) {
			if (fadingIn == true) {
				//important, enable before disable because if a texture is shared between
				//the two modules, it's not unloaded and later loaded again
				moduleOn->enable();
				if (moduleOff != nullptr) {
					moduleOff->disable();
				}

				totalTime += totalTime;
				startTime = SDL_GetTicks();
				fadingIn = false;
			} else {
				startTime = 0;
				App->renderer->clearFaddingEffect();
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
