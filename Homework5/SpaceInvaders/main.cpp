#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <windows.h>
#include "gameTime.h"
#include "GameObject.h"
#include "positionComponents.h"
#include "colorComponents.h"
#include "sizeComponents.h"
#include "movementComponents.h"
#include "Event.h"
#include "eventHandler.h"
#include "charEventHandler.h"
#include "collisionEvent.h"
#include "platfEventHandler.h"
#include "deathEvent.h"
#include "spawnEvent.h"
#include "userInputEvent.h"
#include "eventManager.h"
#include "scriptEvent.h"
#include "ScriptManager.h"
using namespace std;

// Reference: http://zguide.zeromq.org/page:all
std::string s_recv(zmq::socket_t& socket) {
	zmq::message_t message;
	socket.recv(message);
	return std::string(static_cast<char*>(message.data()), message.size());
}
// Reference: http://zguide.zeromq.org/page:all
void s_send(zmq::socket_t& socket, const std::string& string) {
	zmq::message_t message(string.size());
	memcpy(message.data(), string.data(), string.size());
	socket.send(message, zmq::send_flags::dontwait);
}
// Reference: http://zguide.zeromq.org/page:all
void s_sendmore(zmq::socket_t& socket, const std::string& string) {
	zmq::message_t message(string.size());
	memcpy(message.data(), string.data(), string.size());
	socket.send(message, zmq::send_flags::sndmore);
}

// globle boolean variable to detect collision
bool upcoli;
bool downcoli;
bool leftcoli;
bool rightcoli;
bool targetupcoli;
bool targetdowncoli;
bool targetleftcoli;
bool targetrightcoli;
bool pause = false;
bool unpause = false;
float scale = 1.0;

int left_count = 0;
int right_count = 0;
int up_count = 0;
int down_count = 0;

bool isFire = true;

//recording
bool isrecording;
bool isReplay;
sf::Vector2f startRecPosition;
bool scriptingEnable = false;

sf::Vector2f grave(0.f, 1000.f);
bool nextLevel = false;

// instances of needed components
sizeComponents leftBoundSize(10.f, 800.f);
sizeComponents upBoundSize(600.f, 10.f);
sizeComponents rightBoundSize(10.f, 800.f);
sizeComponents downBoundSize(600.f, 10.f);
sizeComponents middleBoundSize(10.f, 800.f);
sizeComponents characterSize(40.f, 40.f);
sizeComponents shipSize(50.f, 50.f);
sizeComponents enemyFireSize(9.f, 9.f);
sizeComponents charFireSize(9.f, 9.f);

positionComponents leftBoundPos(0.f, 0.f);
positionComponents upBoundPos(0.f, 0.f);
positionComponents rightBoundPos(590.f, 0.f);
positionComponents middleBoundPos(300.f, 0.f);
positionComponents downBoundPos(0.f, 790.f);
positionComponents characterPos(300.f, 700.f);
positionComponents shipPos_1(100.f, 100.f);
positionComponents shipPos_2(500.f, 100.f);
positionComponents shipPos_3(200.f, 200.f);
positionComponents shipPos_4(400.f, 200.f);

// ship position for new level
positionComponents new_shipPos_1(250.f, 150.f);
positionComponents new_shipPos_2(400.f, 100.f);
positionComponents new_shipPos_3(200.f, 200.f);
positionComponents new_shipPos_4(450.f, 300.f);

positionComponents SpawnPos(300.f, 700.f);

colorComponents wallColor(128, 128, 128);
colorComponents charactorColor(0, 255, 0);
colorComponents shipColor(255, 0, 0);
colorComponents fireColor(0, 0, 255);
colorComponents enemyfireColor(255, 0, 0);

int main()
{

	sf::Texture ship_t;
	ship_t.loadFromFile("ship.png"); // reference: freepngimg.com

	sf::Texture char_t;
	char_t.loadFromFile("char.png"); // // reference: freepngimg.com

	sf::RenderWindow window(sf::VideoMode(600, 800), "zmei_SpaceInvaders");

	zmq::context_t context(1);
	zmq::socket_t subscriber(context, ZMQ_SUB);
	subscriber.connect("tcp://localhost:5563");
	subscriber.setsockopt(ZMQ_SUBSCRIBE, "1", 1);

	zmq::socket_t client(context, ZMQ_REQ);
	client.connect("tcp://localhost:5564");

	s_send(client, "");
	std::string str = s_recv(client);

	int time = 0;
	std::string control = "";
	int step_size = 500;
	float fps = 60;
	int period = 1000 / fps;
	float velocity = step_size / fps;

	// create boundaries
	GameObject leftBound;
	leftBound.setPositionComponent(leftBoundPos);
	leftBound.setSizeComponent(leftBoundSize);
	leftBound.setColorComponent(wallColor);

	GameObject upBound;
	upBound.setPositionComponent(upBoundPos);
	upBound.setSizeComponent(upBoundSize);
	upBound.setColorComponent(wallColor);

	GameObject rightBound;
	rightBound.setPositionComponent(rightBoundPos);
	rightBound.setSizeComponent(rightBoundSize);
	rightBound.setColorComponent(wallColor);

	GameObject middleBound;
	middleBound.setPositionComponent(middleBoundPos);
	middleBound.setSizeComponent(middleBoundSize);

	GameObject downBound;
	downBound.setPositionComponent(downBoundPos);
	downBound.setSizeComponent(downBoundSize);
	downBound.setColorComponent(wallColor);

	// create multiple space ships
	GameObject ship1;
	ship1.setSizeComponent(shipSize);
	ship1.setPositionComponent(shipPos_1);
	ship1.setColorComponent(shipColor);

	GameObject ship2;
	ship2.setSizeComponent(shipSize);
	ship2.setPositionComponent(shipPos_2);
	ship2.setColorComponent(shipColor);

	GameObject ship3;
	ship3.setSizeComponent(shipSize);
	ship3.setPositionComponent(shipPos_3);
	//ship3.setColorComponent(shipColor);

	GameObject ship4;
	ship4.setSizeComponent(shipSize);
	ship4.setPositionComponent(shipPos_4);
	//ship4.setColorComponent(shipColor);

	// create characters
	GameObject character;
	character.setSizeComponent(characterSize);
	character.setPositionComponent(characterPos);
	//character.setColorComponent(charactorColor);

	//enemy fire
	GameObject enemyFire1;
	enemyFire1.setSizeComponent(enemyFireSize);
	enemyFire1.setColorComponent(enemyfireColor);
	enemyFire1.setPositionComponent(shipPos_1);

	GameObject enemyFire2;
	enemyFire2.setSizeComponent(enemyFireSize);
	enemyFire2.setColorComponent(enemyfireColor);
	enemyFire2.setPositionComponent(shipPos_2);

	GameObject enemyFire3;
	enemyFire3.setSizeComponent(enemyFireSize);
	enemyFire3.setColorComponent(enemyfireColor);
	enemyFire3.setPositionComponent(shipPos_3);

	GameObject enemyFire4;
	enemyFire4.setSizeComponent(enemyFireSize);
	enemyFire4.setColorComponent(enemyfireColor);
	enemyFire4.setPositionComponent(shipPos_4);

	GameObject charFire;
	charFire.setSizeComponent(enemyFireSize);
	charFire.setColorComponent(fireColor);
	charFire.setPositionComponent(characterPos);

	// Spawn points
	GameObject SpawnPoints;
	SpawnPoints.setSizeComponent(characterSize);
	SpawnPoints.setPositionComponent(SpawnPos);
	SpawnPoints.setColorComponent(charactorColor);

	eventManager em;
	vector<Event*> eventQueue = em.getQueue();

	while (window.isOpen())
	{

		gameTime gt = gameTime(step_size);

		eventHandler* eh_character = new charEventHandler();
		Event* fireOutBound = new collisionEvent(charFire, upBound);
		Event* enemyFireOutBound_1 = new collisionEvent(enemyFire1, downBound);
		Event* enemyFireOutBound_2 = new collisionEvent(enemyFire2, downBound);
		Event* enemyFireOutBound_3 = new collisionEvent(enemyFire3, downBound);
		Event* enemyFireOutBound_4 = new collisionEvent(enemyFire4, downBound);

		Event* char_colid_leftBound = new collisionEvent(character, leftBound);
		Event* char_colid_rightBound = new collisionEvent(character, rightBound);
		Event* char_colid_downBound = new collisionEvent(character, downBound);
		// character collision upBound
		Event* char_colid_upBound = new collisionEvent(character, upBound);
		// ships movements and collision
		Event* ship1_colid_left = new collisionEvent(ship1, leftBound);
		Event* ship1_colid_right = new collisionEvent(ship1, middleBound);

		Event* ship2_colid_left = new collisionEvent(ship2, middleBound);
		Event* ship2_colid_right = new collisionEvent(ship2, rightBound);

		Event* ship3_colid_left = new collisionEvent(ship3, leftBound);
		Event* ship3_colid_right = new collisionEvent(ship3, middleBound);

		Event* ship4_colid_left = new collisionEvent(ship4, middleBound);
		Event* ship4_colid_right = new collisionEvent(ship4, rightBound);

		// character death when collision with enemy fire
		Event* char_death_coli_enemyFire1 = new deathEvent(character, enemyFire1);
		Event* char_death_coli_enemyFire2 = new deathEvent(character, enemyFire2);
		Event* char_death_coli_enemyFire3 = new deathEvent(character, enemyFire3);
		Event* char_death_coli_enemyFire4 = new deathEvent(character, enemyFire4);
		Event* char_spawn = new spawnEvent(character, SpawnPoints);
		// enemy ships death collide with character fire
		Event* ship1_death_coli_charFire = new deathEvent(ship1, charFire);
		Event* ship2_death_coli_charFire = new deathEvent(ship2, charFire);
		Event* ship3_death_coli_charFire = new deathEvent(ship3, charFire);
		Event* ship4_death_coli_charFire = new deathEvent(ship4, charFire);

		// user inputs
		Event* key_Left = new userInputEvent(sf::Keyboard::Key::Left);
		Event* key_Right = new userInputEvent(sf::Keyboard::Key::Right);
		Event* key_Up = new userInputEvent(sf::Keyboard::Key::Up);
		Event* key_Down = new userInputEvent(sf::Keyboard::Key::Down);
		Event* key_Space = new userInputEvent(sf::Keyboard::Key::Space);
		Event* key_R = new userInputEvent(sf::Keyboard::Key::R);
		Event* key_S = new userInputEvent(sf::Keyboard::Key::S);
		Event* key_E = new userInputEvent(sf::Keyboard::Key::E);
		Event* key_M = new userInputEvent(sf::Keyboard::Key::M);
		// script event
		Event* scriptOn = new scriptEvent();

		// walls
		sf::RectangleShape leftBound_shape = leftBound.drawObject();
		sf::RectangleShape upBound_shape = upBound.drawObject();
		sf::RectangleShape rightBound_shape = rightBound.drawObject();
		sf::RectangleShape downBound_shape = downBound.drawObject();
		sf::RectangleShape middleBound_shape = middleBound.drawObject();

		// ships
		sf::RectangleShape ship_1 = ship1.drawObject();
		ship_1.setTexture(&ship_t);
		sf::RectangleShape ship_2 = ship2.drawObject();
		ship_2.setTexture(&ship_t);
		sf::RectangleShape ship_3 = ship3.drawObject();
		ship_3.setTexture(&ship_t);
		sf::RectangleShape ship_4 = ship4.drawObject();
		ship_4.setTexture(&ship_t);
	
		// character
		sf::RectangleShape character_1 = character.drawObject();
		character_1.setTexture(&char_t);
		//character_1.setScale(1.5, 1.5);

		// enemyfires
		sf::RectangleShape enemyfire_1 = enemyFire1.drawObject();
		sf::RectangleShape enemyfire_2 = enemyFire2.drawObject();
		sf::RectangleShape enemyfire_3 = enemyFire3.drawObject();
		sf::RectangleShape enemyfire_4 = enemyFire4.drawObject();

		// character fire
		sf::RectangleShape characterfire_1 = charFire.drawObject();
		
		// Spawn Point 1
		sf::RectangleShape SpawnPoints_1 = SpawnPoints.drawObject();

		//// moving platform
		//sf::RectangleShape movePlatform_1 = movingPlatform1.drawObject();
		//sf::RectangleShape movePlatform_2 = movingPlatform2.drawObject();
		//sf::RectangleShape cloud_1 = cloud.drawObject();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (pause == true) {
			velocity = 0;
		}
		else {
			velocity = scale * step_size / fps;
		}

		 //press M to enable scripting for character
		if (eh_character->onEvent(key_M) && scriptingEnable == false) {
			if (eh_character->onEvent(scriptOn)) {
				std::cout << "Scripting Enabled!" << std::endl;
				// call scriptManager
				ScriptManager sm;
				sm.scriptChangeColor();
				colorComponents new_color_fromScript(sm.getR_fromScript(), sm.getG_fromScript(), sm.getB_fromScript());
				sizeComponents new_size_fromScript(sm.getLength_fromScript(), sm.getWidth_fromScript());
				character.setColorComponent(new_color_fromScript);
				character.setSizeComponent(new_size_fromScript);
			}
			scriptingEnable = true;
		}

		// firing
		if (isFire) {
			if (eh_character->onEvent(fireOutBound)) {
				charFire.resetPosition(character_1.getPosition().x, character_1.getPosition().y);
			}
			else {
				charFire.updateUp(25);
			}

			if (eh_character->onEvent(enemyFireOutBound_1)) {
				enemyFire1.resetPosition(ship_1.getPosition().x, ship_1.getPosition().y);
			}
			else {
				enemyFire1.updateDown(25);
			}

			if (eh_character->onEvent(enemyFireOutBound_2)) {
				enemyFire2.resetPosition(ship_2.getPosition().x, ship_2.getPosition().y);
			}
			else {
				enemyFire2.updateDown(25);
			}

			if (eh_character->onEvent(enemyFireOutBound_3)) {
				enemyFire3.resetPosition(ship_3.getPosition().x, ship_3.getPosition().y);
			}
			else {
				enemyFire3.updateDown(25);
			}

			if (eh_character->onEvent(enemyFireOutBound_4)) {
				enemyFire4.resetPosition(ship_4.getPosition().x, ship_4.getPosition().y);
			}
			else {
				enemyFire4.updateDown(25);
			}
		}

		string s = "";
		if (eh_character->onEvent(key_Left)) {
			if (isrecording == true) {
				eventQueue.push_back(key_Left);

			}

			s = "l";
		}
		else if (eh_character->onEvent(key_Right)) {
			if (isrecording == true) {
				eventQueue.push_back(key_Right);
			}
			
			s = "r";
		}
		else if (eh_character->onEvent(key_Up)) {
			if (isrecording == true) {
				eventQueue.push_back(key_Up);
			}
			
			s = "u";
		}
		else if (eh_character->onEvent(key_Down)) {
			if (isrecording == true) {
				eventQueue.push_back(key_Down);
			}
			
			s = "d";
		}

		//std::cout << s << std::endl;
		s_send(client, s);

		string res = s_recv(client);
		//std::cout << res << std::endl;

		string m = s_recv(subscriber);
		string sub = s_recv(subscriber);
		//std::cout << sub << std::endl;

		if (sub.compare("l") == 0) {
			if (eh_character->onEvent(char_colid_leftBound)) {
				// collison
			}
			else {
				character.updateLeft(velocity);
			}
		}
		else if (sub.compare("r") == 0) {
			if (eh_character->onEvent(char_colid_rightBound)) {
				// collison
			}
			else {
				character.updateRight(velocity);
			}
		}
		else if (sub.compare("u") == 0) {
			character.updateUp(velocity);
		}
		else if (sub.compare("d") == 0) {
			if (eh_character->onEvent(char_colid_downBound)) {
				// collison
			}
			else {
				character.updateDown(velocity);
			}
		}

		// handle character death event
		if (eh_character->onEvent(char_death_coli_enemyFire1) || eh_character->onEvent(char_death_coli_enemyFire2)
			|| eh_character->onEvent(char_death_coli_enemyFire3) || eh_character->onEvent(char_death_coli_enemyFire4)) {
			std::cout << "You dead!" << std::endl;
			std::cout << "Start again at the spawn point!!" << std::endl;
			if (eh_character->onEvent(char_spawn)) {
				character.resetPosition(SpawnPoints_1.getPosition().x, SpawnPoints_1.getPosition().y);
			}
		}

		// handle ship1 death
		if (eh_character->onEvent(ship1_death_coli_charFire)) {
			ship1.resetPosition(grave.x, grave.y);
		}
		// handle ship2 death
		if (eh_character->onEvent(ship2_death_coli_charFire)) {
			ship2.resetPosition(grave.x, grave.y);
		}
		// handle ship3 death
		if (eh_character->onEvent(ship3_death_coli_charFire)) {
			ship3.resetPosition(grave.x, grave.y);
		}
		// handle ship4 death
		if (eh_character->onEvent(ship4_death_coli_charFire)) {
			ship4.resetPosition(grave.x, grave.y);
		}
		// mission complete
		if ((ship1.drawObject().getPosition().y >= grave.y && ship2.drawObject().getPosition().y >= grave.y
			&& ship3.drawObject().getPosition().y >= grave.y && ship4.drawObject().getPosition().y >= grave.y) 
			|| (ship1.drawObject().getPosition().y > downBound.drawObject().getPosition().y && ship2.drawObject().getPosition().y > downBound.drawObject().getPosition().y
				&& ship3.drawObject().getPosition().y > downBound.drawObject().getPosition().y && ship4.drawObject().getPosition().y > downBound.drawObject().getPosition().y)) {
			if (nextLevel == false) {
				cout << "Mission complete! Transfering To Next Level!" << endl;
				nextLevel = true;
			}
			character.updateUp(10);
			if (eh_character->onEvent(char_colid_upBound)) {
				character.resetPosition(SpawnPoints_1.getPosition().x, SpawnPoints_1.getPosition().y);
				// reset enemy ships back to scene for next level
				ship1.resetPosition(new_shipPos_1.getxposition(), new_shipPos_1.getYposition());
				enemyFire1.resetPosition(new_shipPos_1.getxposition(), new_shipPos_1.getYposition());
				ship2.resetPosition(new_shipPos_2.getxposition(), new_shipPos_2.getYposition());
				enemyFire2.resetPosition(new_shipPos_2.getxposition(), new_shipPos_2.getYposition());
				ship3.resetPosition(new_shipPos_3.getxposition(), new_shipPos_3.getYposition());
				enemyFire3.resetPosition(new_shipPos_3.getxposition(), new_shipPos_3.getYposition());
				ship4.resetPosition(new_shipPos_4.getxposition(), new_shipPos_4.getYposition());
				enemyFire4.resetPosition(new_shipPos_4.getxposition(), new_shipPos_4.getYposition());
			}
		}

		ship1.updateDown(1);
		ship2.updateDown(1);
		ship3.updateDown(1);
		ship4.updateDown(1);

		// ship1 movement
		if (eh_character->onEvent(ship1_colid_left) || eh_character->onEvent(ship2_colid_left)
			|| eh_character->onEvent(ship3_colid_left) || eh_character->onEvent(ship4_colid_left)) {
			targetleftcoli = true;
			targetrightcoli = false;
		}
		else if (eh_character->onEvent(ship1_colid_right) || eh_character->onEvent(ship2_colid_right)
			|| eh_character->onEvent(ship3_colid_right) || eh_character->onEvent(ship4_colid_right)) {
			targetrightcoli = true;
			targetleftcoli = false;
		}
		if (targetleftcoli == true) {
			ship1.updateRight(2);
			ship2.updateRight(2);
			ship3.updateRight(2);
			ship4.updateRight(2);
			if (targetrightcoli == true) {
				targetleftcoli = false;
			}
		}
		else {
			ship1.updateLeft(2);
			ship2.updateLeft(2);
			ship3.updateLeft(2);
			ship4.updateLeft(2);
		}
		
		if (eventQueue.size() != 0) {
			for (auto i = eventQueue.begin(); i != eventQueue.end(); i++) {
				if (*i == key_Left) {
					left_count++;
					std::cout << "record_left" << std::endl;
				}
				else if (*i == key_Right) {
					right_count++;
					std::cout << "record_right" << std::endl;
				}
				else if (*i == key_Up) {
					up_count++;
					std::cout << "record_up" << std::endl;
				}
				else if (*i == key_Down) {
					down_count++;
					std::cout << "record_down" << std::endl;
				}
			}
		}

		int deltaT = gt.getTime();
		//std::cout << deltaT << std::endl;
		window.clear();
		//window.draw(leftBound_shape);
		//window.draw(upBound_shape);
		//window.draw(rightBound_shape);
		//window.draw(downBound_shape);
		//window.draw(middleBound_shape);
		window.draw(character_1);
		window.draw(ship_1);
		window.draw(ship_2);
		window.draw(ship_3);
		window.draw(ship_4);
		window.draw(characterfire_1);
		window.draw(enemyfire_1);
		window.draw(enemyfire_2);
		window.draw(enemyfire_3);
		window.draw(enemyfire_4);
		//window.draw(SpawnPoints_1);
		window.display();

		if (deltaT <= period) {
			Sleep(period - deltaT);
		}
		else if (deltaT > period) {
			Sleep(2 * period - deltaT);
		}
	}

	return 0;
}
