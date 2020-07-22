#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class colorComponents
{
private:
	int r = 0;
	int g = 0;
	int b = 0;
public:
	colorComponents(int r, int g, int b);
	int getR();
	int getG();
	int getB();
};

