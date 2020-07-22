#pragma once
#include "Event.h"

class collisionEvent : public Event
{
private:
	GameObject a;
	GameObject b;
public:
	collisionEvent(GameObject a, GameObject b);
	GameObject getObjA();
	GameObject getObjB();
	eventType getType();
	char getKeyPress();
};

