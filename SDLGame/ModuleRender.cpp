#include "ModuleRender.h"
#include "Globals.h"
#include "SDL\SDL.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleRender::ModuleRender() {}

// Destructor
ModuleRender::~ModuleRender() {}

// Called before render is available
bool ModuleRender::Init() {
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if (VSYNC == true) {
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);

	if (renderer == NULL) {
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleRender::PreUpdate() {
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update() {
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate() {
	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp() {
	LOG("Destroying renderer");

	//Destroy window
	if (renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section) {
	bool ret = true;
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;

	if (section != NULL) {
		rect.w = section->w;
		rect.h = section->h;
	}
	else {
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	if (SDL_RenderCopy(renderer, texture, section, &rect) != 0) {
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}
