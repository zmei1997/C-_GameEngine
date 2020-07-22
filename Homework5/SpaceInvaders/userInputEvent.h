#pragma once
#include "Event.h"
class userInputEvent : public Event
{
private:
	char key;
	GameObject a;
	GameObject b;
public:
	userInputEvent(char key);
	userInputEvent(GameObject a, GameObject b);
	char getKeyPress();
	GameObject getObjA();
	GameObject getObjB();
	eventType getType();
};

