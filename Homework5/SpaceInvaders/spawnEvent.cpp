#include "spawnEvent.h"

spawnEvent::spawnEvent(GameObject a, GameObject b) {
	this->a = a;
	this->b = b;
}

GameObject spawnEvent::getObjA() {
	return a;
}

GameObject spawnEvent::getObjB() {
	return b;
}

eventType spawnEvent::getType() {
	return spawnEvent_type;
}

char spawnEvent::getKeyPress() {
	return NULL;
}
