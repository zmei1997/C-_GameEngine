#include "movementComponents.h"

movementComponents::movementComponents(float v) {
	this->velocity = v;
}

float movementComponents::getVelocity() {
	return velocity;
}