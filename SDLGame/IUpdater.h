#pragma once
#ifndef IUpdater_H
#define IUpdater_H

enum update_status {
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};
struct IUpdater {
	inline virtual bool start() {
		return true;
	}

	inline virtual update_status preUpdate() {
		return UPDATE_CONTINUE;
	}

	inline virtual update_status update() {
		return UPDATE_CONTINUE;
	}

	inline virtual update_status postUpdate() {
		return UPDATE_CONTINUE;
	}

	inline virtual bool cleanUp() {
		return true;
	}
};
#endif // !IUpdater_H
