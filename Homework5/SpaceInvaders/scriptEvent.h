#pragma once
#include "Event.h"

class scriptEvent : public Event
{
private:
	GameObject a;
	GameObject b;
public:
	scriptEvent();
	scriptEvent(GameObject a, GameObject b);
	GameObject getObjA();
	GameObject getObjB();
	eventType getType();
	char getKeyPress();
};

