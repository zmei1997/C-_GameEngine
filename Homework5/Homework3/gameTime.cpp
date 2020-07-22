#include "gameTime.h"
time_point<system_clock> epoch;
gameTime::gameTime(int step_size) {
	this->step_size = step_size;
	start_time = duration_cast<milliseconds>(epoch - system_clock::now());
}

int gameTime::getTime() {
	milliseconds now = duration_cast<milliseconds>(epoch - system_clock::now());
	milliseconds ea = now - start_time;
	int ellapsed = (int)ea.count() / step_size;
	return ellapsed;
}
