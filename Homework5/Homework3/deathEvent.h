#pragma once
#include "Event.h"
class deathEvent : public Event
{
private:
	GameObject a;
	GameObject b;
public:
	deathEvent(GameObject a, GameObject b);
	GameObject getObjA();
	GameObject getObjB();
	eventType getType();
	char getKeyPress();
};

