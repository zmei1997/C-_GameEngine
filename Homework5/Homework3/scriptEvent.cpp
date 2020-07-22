#include "scriptEvent.h"

scriptEvent::scriptEvent() {}

scriptEvent::scriptEvent(GameObject a, GameObject b) {
	this->a = a;
	this->b = b;
}

GameObject scriptEvent::getObjA() {
	return a;
}

GameObject scriptEvent::getObjB() {
	return b;
}

eventType scriptEvent::getType() {
	return scriptEvent_type;
}

char scriptEvent::getKeyPress() {
	return NULL;
}
