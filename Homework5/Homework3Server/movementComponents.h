#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class movementComponents
{
private:
	float velocity;
public:
	movementComponents(float v);
	float getVelocity();
};

