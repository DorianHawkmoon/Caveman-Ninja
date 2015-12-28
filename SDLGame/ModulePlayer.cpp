#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/SDL.h"

#include "State.h"
#include "ConditionCallback.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA
ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
{
	position.x = 100;
	//altura de la escena menos la distancia desde el suelo(borde inferior de la pantalla)
	//hasta donde quiero poner a joe
	position.y = 256-28;

	Animation forward;
	Animation idle;
	/*Animation backward;
	Animation forward;
	Animation* actual;*/
	idle.frames.push_back({0, 0, 28, 47});

	forward.frames.push_back({0, 102, 42, 48});
	forward.frames.push_back({42, 102, 42, 48});
	forward.frames.push_back({84, 102, 42, 48});
	forward.frames.push_back({126, 102, 42, 48});
	forward.speed = 0.05f;

	State<Animation>* forwardAnimation = new State<Animation>(forward);
	State<Animation>* idleAnimation = new State<Animation>(idle);
	animations = new StateMachine<Animation>(idleAnimation);
	animations->addState(forwardAnimation);


	auto toForward = []() {
		return App->input->GetKey(SDL_SCANCODE_D) == (KEY_DOWN || KEY_REPEAT);
	};
	auto toIdle = []() {
		return (App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE);
	};


	Condition* conditionForward = new ConditionCallback(toForward);
	Condition* conditionIdle = new ConditionCallback(toIdle);

	StateTransition<Animation>* transitionForward = new StateTransition<Animation>(forwardAnimation);
	transitionForward->addCondition(conditionForward);
	idleAnimation->addTransition(transitionForward);

	StateTransition<Animation>* transitionIdle = new StateTransition<Animation>(idleAnimation);
	transitionIdle->addCondition(conditionIdle);
	forwardAnimation->addTransition(transitionIdle);
}

ModulePlayer::~ModulePlayer()
{
	// Homework : check for memory leaks
}

// Load assets
bool ModulePlayer::start(){
	LOG("Loading player");

	graphics = App->textures->load("joe.png"); // arcade version
	//actual = &idle;
	return true;
}

// Unload assets
bool ModulePlayer::cleanUp(){
	LOG("Unloading player");

	App->textures->unload(graphics);

	return true;
}

// Update
update_status ModulePlayer::update(){
	// TODO 9: Draw the player with its animation
	// make sure to detect player movement and change its
	
	int movement = 0;
	if (App->input->GetKey(SDL_SCANCODE_D)) {
 		movement = 1;
	} else if (App->input->GetKey(SDL_SCANCODE_A)) {
		movement = -1;
	}
	position.x += movement;

	/*switch (movement) {
		case 0:
			actual = &idle;
			break;
		case 1:
			actual = &forward;
			break;
		case -1:
			actual = &backward;
			break;
	}*/

	animations->proccessState();

	return UPDATE_CONTINUE;
}

update_status ModulePlayer::postUpdate() {
	// position while cycling the animation(check Animation.h)
	iPoint pos = position;
	Animation* anim = animations->getState()->getValue();
	pos.y -= anim->GetCurrentFrame().h;
	App->renderer->blit(graphics, pos, &(anim->GetCurrentFrame()));
	SDL_Color color;
	color.r = 255;
	color.g = 165;
	color.b = 0;
	color.a = 127;
	App->renderer->paintRectangle(color, pos, anim->GetCurrentFrame());
	return UPDATE_CONTINUE;
}