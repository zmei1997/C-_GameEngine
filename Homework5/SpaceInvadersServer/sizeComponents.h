#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class sizeComponents
{
private:
	float length;
	float width;
public:
	sizeComponents(float length, float width);
	float getSizeLength();
	float getSizeWidth();
};

