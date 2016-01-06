#include "ModuleRender.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Globals.h"
#include "ModuleInput.h"
#include "SDL/SDL.h"
#include "ModuleTextures.h"
#include <math.h>


ModuleRender::ModuleRender(bool started ): 
	Module(started), renderer(nullptr), camera({SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT* SCREEN_SIZE}) {

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
	circle = App->textures->load("circle_64.png");
	rectangle = App->textures->load("square_64.png");
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

	if (App->input->getKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		camera.setY(camera.getY() - speed);
	}

	if (App->input->getKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		camera.setY(camera.getY() + speed);
	}

	if (App->input->getKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		camera.setX(camera.getX() - speed);
	}

	if (App->input->getKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
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

bool ModuleRender::blit(SDL_Texture * texture, const Transform& transform, const SDL_Rect* sectionTexture,  float speed) {
	bool result = true;

	SDL_Rect cam = camera.getViewArea(speed);

	SDL_Rect rectDestiny;
	iPoint pos = {(int)transform.position.x, (int)transform.position.y};
	rectDestiny.x = static_cast<int>( pos.x  * SCREEN_SIZE - cam.x);
	rectDestiny.y = static_cast<int>( pos.y  * SCREEN_SIZE - cam.y);
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
		if (SDL_RenderCopyEx(renderer, texture, sectionTexture, &rectDestiny,
								0.0f, nullptr, transform.flip) != 0) {
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			result = false;
		}
	}

	return result;
}

bool ModuleRender::paintCollision(const Collider * collision) {
	collision->paintCollider();
	return true;
}

bool ModuleRender::paintRectangle(const SDL_Color& color, const iPoint& position, const iPoint& rect, float speed) {
	bool result = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	int x = static_cast<int>(position.x * SCREEN_SIZE - camera.getX(speed));
	int y = static_cast<int>(position.y * SCREEN_SIZE - camera.getY(speed));
	int w = static_cast<int>(rect.x * SCREEN_SIZE);
	int h = static_cast<int>(rect.y * SCREEN_SIZE);

	SDL_Rect rectResult = {0,0,0,0};
	rectResult.w = w;
	rectResult.h = h;
	rectResult.x = x;
	rectResult.y = y;


	if (SDL_RenderFillRect(renderer, &rectResult) != 0) {
		LOG("Cannot draw rectangle to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		result = false;
	}

	
	return result;
}

bool ModuleRender::paintRectangle(const SDL_Color & color, const Transform & transform, const iPoint & rect, float speed) {
	// Determina la posición del dibujo en pantalla
	//renderPosition += offset.Rotate(transform->GetGlobalRotation());
	//escala de la textura con tamaño real
	fPoint renderScale = fPoint(rect.x / 64.0f, rect.y/ 64.0f);



	// Determina el área donde pintar en función de la escala
	SDL_Rect cam = camera.getViewArea(speed);

	SDL_Rect rectDestiny;
	iPoint pos = {(int) transform.position.x, (int) transform.position.y};
	rectDestiny.x = static_cast<int>(pos.x  * SCREEN_SIZE - cam.x);
	rectDestiny.y = static_cast<int>(pos.y  * SCREEN_SIZE - cam.y);
	rectDestiny.w = 0;
	rectDestiny.h = 0;

	//if not, ask the whole texture
	SDL_QueryTexture(rectangle, nullptr, nullptr, &rectDestiny.w, &rectDestiny.h);
	
	rectDestiny.w *= SCREEN_SIZE * renderScale.x;
	rectDestiny.h *= SCREEN_SIZE * renderScale.y;

	// Determina el color de la textura
	bool result = true;
	if (SDL_SetTextureColorMod(rectangle, color.r, color.g, color.b)) {
		LOG("Cannot tint texture. SDL_SetTextureColorMod error: %s", SDL_GetError());
		result = false;
	}

	// Determina la transparencia de la textura
	if (SDL_SetTextureAlphaMod(rectangle, color.a)) {
		LOG("Cannot change texture opacity. SDL_SetTextureAlphaMod error: %s", SDL_GetError());
		result = false;
	}

	// Renderiza la textura
	//pivot is nullptr because is middle of rectangle
	if (SDL_RenderCopyEx(renderer, rectangle, nullptr, &rectDestiny, transform.rotation, nullptr, transform.flip) != 0) {
		LOG("Cannot blit to screen. SDL_RenderCopyEx error: %s", SDL_GetError());
		result = false;
	}

	return result;
}

bool ModuleRender::paintCircle(const SDL_Color & color, const fPoint & position, float radius, float speed) {
	bool result = true; 

	iPoint pos;
	pos.x = static_cast<int>((position.x-radius) * SCREEN_SIZE - camera.getX(speed));
	pos.y = static_cast<int>((position.y-radius) * SCREEN_SIZE - camera.getY(speed));

	float scale = radius / 32;

	SDL_Rect rectDestiny;
	rectDestiny.x = pos.x;
	rectDestiny.y = pos.y;
	SDL_QueryTexture(circle, nullptr, nullptr, &rectDestiny.w, &rectDestiny.h);

	rectDestiny.w = static_cast<int>(rectDestiny.w * scale * SCREEN_SIZE);
	rectDestiny.h = static_cast<int>(rectDestiny.h * scale * SCREEN_SIZE);

	if (SDL_SetTextureColorMod(circle, color.r, color.g, color.b)) {
		LOG("Cannot tint texture. SDL_SetTextureColorMod error: %s", SDL_GetError());
		result = false;
	}

	if (SDL_RenderCopy(renderer, circle, nullptr, &rectDestiny) != 0) {
		LOG("Cannot draw circle to screen. Error: %s", SDL_GetError());
		result = false;
	}

	return result;
}
