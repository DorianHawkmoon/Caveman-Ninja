#include "SDL\SDL.h"
#include "Globals.h"
#include "Application.h"

enum main_states {
	MAIN_CREATION,
	MAIN_INIT,
	MAIN_START,
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
				state = MAIN_INIT;
				break;

			case MAIN_INIT:

				LOG("Application Init --------------");
				if (App->Init() == false) {
					LOG("Application Init exits with error -----");
					state = MAIN_EXIT;
				} else {
					state = MAIN_START;
				}

				break;

			case MAIN_START:

				LOG("Application Start --------------");
				if (App->Start() == false) {
					LOG("Application Start exits with error -----");
					state = MAIN_EXIT;
				} else {
					state = MAIN_UPDATE;
					LOG("Application Update --------------");
				}

				break;

			case MAIN_UPDATE:
			{
				int update_return = App->PreUpdate();
				if (update_return == UPDATE_ERROR) {
					LOG("Application PreUpdate exits with error -----");
					state = MAIN_EXIT;
				} else if (update_return == UPDATE_STOP) {
					state = MAIN_FINISH;
				}

				update_return = App->Update();
				if (update_return == UPDATE_ERROR) {
					LOG("Application Update exits with error -----");
					state = MAIN_EXIT;
				} else if (update_return == UPDATE_STOP) {
					state = MAIN_FINISH;
				}

				update_return = App->PostUpdate();
				if (update_return == UPDATE_ERROR) {
					LOG("Application PostUpdate exits with error -----");
					state = MAIN_EXIT;
				} else if (update_return == UPDATE_STOP) {
					state = MAIN_FINISH;
				}
				break;

			}


			case MAIN_FINISH:

				LOG("Application CleanUp --------------");
				if (App->CleanUp() == false) {
					LOG("Application CleanUp exits with error -----");
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
int mainCRTStartup(int argc, char* argv[]) {
	return main(argc, argv);
}

//windows entry window
int WinMainCRTStartup(int argc, char* argv[]) {
	return main(argc, argv);
}

