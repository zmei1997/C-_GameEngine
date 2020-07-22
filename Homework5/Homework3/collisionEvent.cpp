#include "collisionEvent.h"

collisionEvent::collisionEvent(GameObject a, GameObject b) {
	this->a = a;
	this->b = b;
}

GameObject collisionEvent::getObjA() {
	return a;
}

GameObject collisionEvent::getObjB() {
	return b;
}

eventType collisionEvent::getType()
{
	return collisionEvent_type;
}

char collisionEvent::getKeyPress(){
	return NULL;
}
