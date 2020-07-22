#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "positionComponents.h"
#include "positionComponents.h"
#include "colorComponents.h"
#include "sizeComponents.h"
#include "movementComponents.h"

class GameObject
{
private:
	// new instance of object
	sf::RectangleShape obj;

public:
	GameObject();
	sf::RectangleShape drawObject();
	void setSizeComponent(sizeComponents c);
	void setPositionComponent(positionComponents c);
	void setColorComponent(colorComponents c);
	void updateLeft(int acceleration);
	void updateRight(int acceleration);
	void updateUp(int acceleration);
	void updateDown(int acceleration);
	void resetPosition(float a, float b);
};

