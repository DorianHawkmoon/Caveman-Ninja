#pragma once

#ifndef MODULE_INPUT_H
#define MODULE_INPUT_H

typedef unsigned __int8 Uint8;
#include "Module.h"

class ModuleInput : public Module {
public:
	ModuleInput();
	virtual ~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

private:
	const Uint8 *keyboard = nullptr;
};

#endif // !MODULE_INPUT_H
