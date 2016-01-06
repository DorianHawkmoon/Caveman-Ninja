#pragma once
#include "Module.h"
#include <list>
class Entity;

class ModuleEnemy :	public Module {
public:
	ModuleEnemy(bool start_enabled = true);
	virtual ~ModuleEnemy();

	void registerEnemy(Entity* enemy);
	void unregisterEnemy(Entity* enemy);

	bool start();
	update_status preUpdate();
	update_status update();
	update_status postUpdate();
	bool cleanUp();

public:
	//TODO cambiar, implica cambios en scene node para que no elimine entidades
	//con lo que este y escena se encargan de guardar y eliminar las entidades

	//not owned!! owned by scene
	//the module only manage the behaviour
	std::list<Entity*> enemies;

};

