#include "platfEventHandler.h"

platfEventHandler::platfEventHandler() {}

bool platfEventHandler::onEvent(Event* e) {
	eventType et = e->getType();
	switch (et)
	{
	case collisionEvent_type:
		return (e->getObjA()).drawObject().getGlobalBounds().intersects((e->getObjB()).drawObject().getGlobalBounds());
		break;
	case scriptEvent_type:
		return true;
		break;
	default:
		break;
	}
}