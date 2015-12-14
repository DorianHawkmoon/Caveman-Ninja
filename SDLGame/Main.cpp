#include "SDL\SDL.h"
#include "Globals.h"
#include "Application.h"

enum main_states {
	MAIN_CREATION,
	MAIN_init,
	MAIN_start,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = nullptr;

//Standard c++, gcc, default
int main(int argc, char* argv[]) {
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	while (state != MAIN_EXIT) {
		switch (state) {
			case MAIN_CREATION:

				LOG("Application Creation --------------");
				App = new Application();
				state = MAIN_init;
				break;

			case MAIN_init:

				LOG("Application init --------------");
				if (App->init() == false) {
					LOG("Application init exits with error -----");
					state = MAIN_EXIT;
				} else {
					state = MAIN_start;
				}

				break;

			case MAIN_start:

				LOG("Application start --------------");
				if (App->start() == false) {
					LOG("Application start exits with error -----");
					state = MAIN_EXIT;
				} else {
					state = MAIN_UPDATE;
					LOG("Application update --------------");
				}

				break;

			case MAIN_UPDATE:
			{
				int update_return = App->preUpdate();
				if (update_return == UPDATE_ERROR) {
					LOG("Application preUpdate exits with error -----");
					state = MAIN_EXIT;
				} else if (update_return == UPDATE_STOP) {
					state = MAIN_FINISH;
				}

				update_return = App->update();
				if (update_return == UPDATE_ERROR) {
					LOG("Application update exits with error -----");
					state = MAIN_EXIT;
				} else if (update_return == UPDATE_STOP) {
					state = MAIN_FINISH;
				}

				update_return = App->postUpdate();
				if (update_return == UPDATE_ERROR) {
					LOG("Application postUpdate exits with error -----");
					state = MAIN_EXIT;
				} else if (update_return == UPDATE_STOP) {
					state = MAIN_FINISH;
				}
				break;

			}


			case MAIN_FINISH:

				LOG("Application cleanUp --------------");
				if (App->cleanUp() == false) {
					LOG("Application cleanUp exits with error -----");
				} else
					main_return = EXIT_SUCCESS;



				state = MAIN_EXIT;

				break;

		}
	}

	if (App != nullptr) {
		delete App;
		App = nullptr;
	}
	LOG("Bye :)\n");
	return main_return;
}

//windows entry console
int mainCRTstartup(int argc, char* argv[]) {
	return main(argc, argv);
}

//windows entry window
int WinMainCRTstartup(int argc, char* argv[]) {
	return main(argc, argv);
}

