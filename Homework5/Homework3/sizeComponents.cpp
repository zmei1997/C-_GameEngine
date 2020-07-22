#include "sizeComponents.h"

sizeComponents::sizeComponents(float length, float width) {
	this->length = length;
	this->width = width;
}

float sizeComponents::getSizeLength() {
	return length;
}

float sizeComponents::getSizeWidth() {
	return width;
}