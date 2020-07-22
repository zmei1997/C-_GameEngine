#pragma once
#include "Event.h"
class spawnEvent : public Event
{
private:
	GameObject a;
	GameObject b;
public:
	spawnEvent(GameObject a, GameObject b);
	GameObject getObjA();
	GameObject getObjB();
	eventType getType();
	char getKeyPress();
};

