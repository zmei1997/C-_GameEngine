#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class positionComponents
{
private:
	float xposition;
	float yposition;
public:
	positionComponents(float x, float y);
	float getxposition();
	float getYposition();
};

