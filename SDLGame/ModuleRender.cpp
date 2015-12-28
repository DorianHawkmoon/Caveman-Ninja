#include "ModuleRender.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Globals.h"
#include "ModuleInput.h"
#include "SDL/SDL.h"
#include <math.h>

ModuleRender::ModuleRender(bool started ): 
	Module(started), renderer(nullptr), camera(SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT* SCREEN_SIZE) {

}

// Destructor
ModuleRender::~ModuleRender() {
	LOG("Destroying renderer");

	//Destroy window
	if (renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
	}
}

// Called before render is available
bool ModuleRender::init() {
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

bool ModuleRender::start() {
	return true;
}

// Called every draw update
update_status ModuleRender::preUpdate() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::update() {

	// debug camera
	int speed = 1;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		camera.setY(camera.getY() - speed);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		camera.setY(camera.getY() + speed);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		camera.setX(camera.getX() - speed);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		camera.setX(camera.getX() + speed);
	}

	return UPDATE_CONTINUE;
}

update_status ModuleRender::postUpdate() {
	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::cleanUp() {
	LOG("CleanUp render");
	return true;
}

bool ModuleRender::blit(SDL_Texture * texture, iPoint position, const SDL_Rect* sectionTexture, float speed) {
	bool result = true;

	SDL_Rect cam = camera.getViewArea(speed);

	SDL_Rect rectDestiny;
	rectDestiny.x = static_cast<int>( position.x * SCREEN_SIZE - cam.x);
	rectDestiny.y = static_cast<int>( position.y * SCREEN_SIZE - cam.y);
	rectDestiny.w = 0;
	rectDestiny.h = 0;

	//need the size of the image to paint at it image
	if (sectionTexture != nullptr) {
		rectDestiny.w = sectionTexture->w;
		rectDestiny.h = sectionTexture->h;
	} else {
		//if not, ask the whole texture
		SDL_QueryTexture(texture, nullptr, nullptr, &rectDestiny.w, &rectDestiny.h);
	}
	rectDestiny.w *= SCREEN_SIZE;
	rectDestiny.h *= SCREEN_SIZE;

	//check if are inside the view
	SDL_Rect sizeWindows = camera.getWindowsSize();
	if (SDL_HasIntersection(&sizeWindows, &rectDestiny) == SDL_TRUE) {
		//paint
		if (SDL_RenderCopy(renderer, texture, sectionTexture, &rectDestiny) != 0) {
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			result = false;
		}
	}

	return result;
}

//inline bool ModuleRender::paintCollision(const ICollider * collision) {
//	collision->paintCollider();
//}

bool ModuleRender::paintRectangle(const SDL_Color& color, const iPoint& position, const SDL_Rect& rect, float speed) {
	bool result = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	SDL_Rect rec(rect);
	rec.x = static_cast<int>(position.x * SCREEN_SIZE -  camera.getX(speed));
	rec.y = static_cast<int>(position.y * SCREEN_SIZE - camera.getY(speed));
	rec.w *= SCREEN_SIZE;
	rec.h *= SCREEN_SIZE;
	

	if (SDL_RenderFillRect(renderer, &rec) != 0) {
		LOG("Cannot draw rectangle to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		result = false;
	}



	return result;
}
