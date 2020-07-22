#include "deathEvent.h"

deathEvent::deathEvent(GameObject a, GameObject b) {
	this->a = a;
	this->b = b;
}

GameObject deathEvent::getObjA() {
	return a;
}

GameObject deathEvent::getObjB() {
	return b;
}

eventType deathEvent::getType()
{
	return deathEvent_type;
}

char deathEvent::getKeyPress() {
	return NULL;
}