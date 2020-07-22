#include "userInputEvent.h"

userInputEvent::userInputEvent(char key){
	this->key = key;
}

userInputEvent::userInputEvent(GameObject a, GameObject b) {
	this->a = a;
	this->b = b;
}

char userInputEvent::getKeyPress() {
	return key;
}

GameObject userInputEvent::getObjA() {
	return a;
}

GameObject userInputEvent::getObjB() {
	return b;
}

eventType userInputEvent::getType()
{
	return userInputEvent_type;
}