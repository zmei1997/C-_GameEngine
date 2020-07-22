#pragma once
#include <chrono>
#include "Timeline.h"
using namespace std::chrono;

class gameTime : public Timeline
{
private:
	milliseconds start_time;
	int step_size;
public:
	gameTime(int step_size);
	int getTime();
};