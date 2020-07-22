#include "GameObject.h"

GameObject::GameObject() {
}

sf::RectangleShape GameObject::drawObject() {
	return obj;
}

void GameObject::setSizeComponent(sizeComponents s) {
	obj.setSize(sf::Vector2f(s.getSizeLength(), s.getSizeWidth()));
}

void GameObject::setPositionComponent(positionComponents p) {
	obj.setPosition(p.getxposition(), p.getYposition());
}

void GameObject::setColorComponent(colorComponents c) {
	obj.setFillColor(sf::Color(c.getR(), c.getG(), c.getB(), 255));
}

void GameObject::updateLeft(float acceleration) {
	obj.move(-0.5 * acceleration, 0.0);
	//std::cout << obj.getPosition().x << std::endl;
}
void GameObject::updateRight(float acceleration) {
	obj.move(0.5 * acceleration, 0.0);
}
void GameObject::updateUp(float acceleration) {
	obj.move(0.0, -0.5 * acceleration);
}
void GameObject::updateDown(float acceleration) {
	obj.move(0.0, 0.5 * acceleration);
}

void GameObject::resetPosition(float a, float b) {
	obj.setPosition(a, b);
}

