#pragma once
#ifndef MODULE_RENDER_H
#define MODULE_RENDER_H

#include "Module.h"
#include "SDL/SDL.h"
#include "Point.h"
#include "Collider.h"
#include "Camera.h"
#include "Transform.h"
#include "GUITransform.h"
struct SDL_Texture;
struct SDL_Renderer;
struct Particle;

class ModuleRender : public Module {
public:
	ModuleRender(bool started = true);
	virtual ~ModuleRender();

	bool init();
	bool start();
	update_status preUpdate();
	update_status update();
	update_status postUpdate();
	bool cleanUp();

	bool blit(SDL_Texture* texture, const Transform& transform, const SDL_Rect* sectionTexture, float speed = 1.0f);
	bool paintCollision(const Collider* collision);
	bool paintRectangle(const SDL_Color& color, const iPoint& position, const iPoint& rect, float speed=1.0f);
	bool paintRectangle(const SDL_Color& color, const Transform& transform, const iPoint& rect, const iPoint& pivot = {0,0}, float speed = 1.0f);
	bool paintCircle(const SDL_Color& color, const fPoint& position, float radius, float speed = 1.0f);
	bool paintParticle(const Particle* particle);
	bool insideCamera(const SDL_Rect& one, float speed=1.0f) const;

	bool paintGUI(SDL_Texture* texture, const GUITransform& transform, const SDL_Rect* sectionTexture);

public:
	SDL_Texture* circle;
	SDL_Texture* rectangle;
	SDL_Renderer* renderer;
	Camera camera;

private:
	SDL_Rect getCorrectCamera(float speed) const;
};


#endif // !MODULE_RENDER_H
