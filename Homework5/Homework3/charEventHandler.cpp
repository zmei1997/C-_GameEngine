#include "charEventHandler.h"
#include <iostream>

charEventHandler::charEventHandler(){}

bool charEventHandler::onEvent(Event *e) {
	eventType et = e->getType();
	switch (et)
	{
	case collisionEvent_type:
		return (e->getObjA()).drawObject().getGlobalBounds().intersects((e->getObjB()).drawObject().getGlobalBounds());
		break;
	case deathEvent_type:
		return (e->getObjA()).drawObject().getGlobalBounds().intersects((e->getObjB()).drawObject().getGlobalBounds());
		break;
	case spawnEvent_type:
		return true;
		break;
	case userInputEvent_type:
		if (e->getKeyPress() == sf::Keyboard::Key::Left) {
			return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
		}
		else if (e->getKeyPress() == sf::Keyboard::Key::Right) {
			return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
		}
		else if (e->getKeyPress() == sf::Keyboard::Key::Up) {
			return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
		}
		else if (e->getKeyPress() == sf::Keyboard::Key::Down) {
			return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
		}
		else if (e->getKeyPress() == sf::Keyboard::Key::Space) {
			return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
		}
		else if (e->getKeyPress() == sf::Keyboard::Key::R) {
			return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R);
		}
		else if (e->getKeyPress() == sf::Keyboard::Key::S) {
			return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
		}
		else if (e->getKeyPress() == sf::Keyboard::Key::E) {
			return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);
		}
		else if (e->getKeyPress() == sf::Keyboard::Key::M) {
			return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M);
		}
		break;
	case scriptEvent_type:
		return true;
		break;
	default:
		break;
	}
}