#pragma once
#include "GameObject.h"
#include <string>

enum eventType {
	collisionEvent_type,
	deathEvent_type,
	spawnEvent_type,
	userInputEvent_type,
	scriptEvent_type,
};

class Event
{
public:
	virtual GameObject getObjA() = 0;
	virtual GameObject getObjB() = 0;
	virtual eventType getType() = 0;
	virtual char getKeyPress() = 0;
};

