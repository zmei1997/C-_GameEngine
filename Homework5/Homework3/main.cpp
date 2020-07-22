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
#include "scriptObject.h"
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
int halfSpeed = 0;
int normalSpeed = 1;
int doubleSpeed = 0;

int left_count = 0;
int right_count = 0;
int up_count = 0;
int down_count = 0;

//recording
bool isrecording;
bool isReplay;
sf::Vector2f startRecPosition;

bool scriptingEnable = false;

// instances of needed components
sizeComponents upWallSize(800.f, 10.f);
sizeComponents side_boundarySize(10.f, 600.f);
sizeComponents death_zoneSize(1600.f, 1.f);
sizeComponents groundSize(300.f, 100.f);
sizeComponents characterSize(30.0f, 30.0f);
sizeComponents movingPlatformSize(100.f, 10.f);
sizeComponents cloudSize(200.f, 100.f);

positionComponents side_boundary_leftPos(0.f, 0.f);
positionComponents side_boundary_rightPos(400.f, 0.f);
positionComponents upWallPos(0.f, 250.f);
positionComponents rightWallPos(790.f, 0.f);
positionComponents death_zonePos(0.f, 599.f);
positionComponents groundPos(0.f, 500.f);
positionComponents characterPos(20.f, 465.f);
positionComponents movingPlatformPos1(400.f, 400.f);
positionComponents movingPlatformPos2(650.f, 550.f);
positionComponents SpawnPos(200.f, 465.f);
positionComponents cloudpos(150.f, 30.f);

colorComponents wallColor(128, 128, 128);
colorComponents charactorColor(0, 255, 0);
colorComponents movingPlatformColor(255, 0, 0);

sizeComponents level2_Ground_size(300.f, 50.f);
positionComponents level2_Ground_pos(900.f, 200.f);

int main()
{
	
	sf::RenderWindow window(sf::VideoMode(800, 600), "zmei_HW");

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

	sf::Texture cloud_t;
	cloud_t.loadFromFile("cloud.png"); // Reference: pngimg.com/download/4323
	//sf::Texture tank;
	//tank.loadFromFile("tank.png"); // Reference: pngimg.com/download/4323

	// create multiple staticPlatforms
	GameObject staticPlatform1; // left wall
	staticPlatform1.setSizeComponent(side_boundarySize);
	staticPlatform1.setPositionComponent(side_boundary_leftPos);
	staticPlatform1.setColorComponent(wallColor);

	GameObject staticPlatform2; // up wall
	staticPlatform2.setSizeComponent(upWallSize);
	staticPlatform2.setPositionComponent(upWallPos);
	staticPlatform2.setColorComponent(wallColor);

	GameObject staticPlatform5; // platform
	staticPlatform5.setSizeComponent(groundSize);
	staticPlatform5.setPositionComponent(groundPos);
	staticPlatform5.setColorComponent(wallColor);

	// create side_boundary object
	GameObject side_boundary; // side_boundary
	side_boundary.setSizeComponent(side_boundarySize);
	side_boundary.setPositionComponent(rightWallPos);

	// create death_zone object
	GameObject death_zone; // death zone
	death_zone.setSizeComponent(death_zoneSize);
	death_zone.setPositionComponent(death_zonePos);
	death_zone.setColorComponent(wallColor);

	// create characters
	GameObject character;
	character.setSizeComponent(characterSize);
	character.setPositionComponent(characterPos);
	character.setColorComponent(charactorColor);

	// create movingPlatforms
	GameObject movingPlatform1;
	movingPlatform1.setSizeComponent(movingPlatformSize);
	movingPlatform1.setPositionComponent(movingPlatformPos1);
	movingPlatform1.setColorComponent(movingPlatformColor);
	GameObject movingPlatform2;
	movingPlatform2.setSizeComponent(movingPlatformSize);
	movingPlatform2.setPositionComponent(movingPlatformPos2);
	movingPlatform2.setColorComponent(movingPlatformColor);
	GameObject cloud;
	cloud.setSizeComponent(cloudSize);
	cloud.setPositionComponent(cloudpos);
	//cloud.setColorComponent(movingPlatformColor);

	// Spawn points
	GameObject SpawnPoints;
	SpawnPoints.setSizeComponent(characterSize);
	SpawnPoints.setPositionComponent(SpawnPos);
	SpawnPoints.setColorComponent(charactorColor);

	// objects for level 2
	GameObject staticPlatform6; // platform
	staticPlatform6.setSizeComponent(level2_Ground_size);
	staticPlatform6.setPositionComponent(level2_Ground_pos);
	staticPlatform6.setColorComponent(wallColor);

	GameObject staticPlatform7; // ground
	staticPlatform7.setSizeComponent(groundSize);
	staticPlatform7.setPositionComponent(level2_Ground_pos);
	staticPlatform7.setColorComponent(wallColor);

	eventManager em;
	vector<Event*> eventQueue = em.getQueue();

	while (window.isOpen())
	{

		gameTime gt = gameTime(step_size);

		eventHandler* eh_character = new charEventHandler();
		Event* coli_ground_1 = new collisionEvent(character, staticPlatform5);
		Event* coli_ground_2 = new collisionEvent(character, staticPlatform6);
		Event* coli_ground_3 = new collisionEvent(character, staticPlatform7);
		Event* coli_mvplatf_1 = new collisionEvent(character, movingPlatform1);
		Event* coli_mvplatf_2 = new collisionEvent(character, movingPlatform2);
		Event* coli_sideBound_left = new collisionEvent(character, staticPlatform1);
		Event* coli_sideBound_right = new collisionEvent(character, side_boundary);

		Event* char_death = new deathEvent(character, death_zone);
		Event* char_spawn = new spawnEvent(character, SpawnPoints);

		Event* key_Left = new userInputEvent(sf::Keyboard::Key::Left);
		Event* key_Right = new userInputEvent(sf::Keyboard::Key::Right);
		Event* key_Up = new userInputEvent(sf::Keyboard::Key::Up);
		Event* key_Down = new userInputEvent(sf::Keyboard::Key::Down);
		Event* key_Space = new userInputEvent(sf::Keyboard::Key::Space);
		Event* key_R = new userInputEvent(sf::Keyboard::Key::R);
		Event* key_S = new userInputEvent(sf::Keyboard::Key::S);
		Event* key_E = new userInputEvent(sf::Keyboard::Key::E);
		Event* key_M = new userInputEvent(sf::Keyboard::Key::M);

		eventHandler* eh_platform = new platfEventHandler();
		Event* mvp1_coli_char = new collisionEvent(movingPlatform1, character);
		Event* mvp1_coli_upwall = new collisionEvent(movingPlatform1, staticPlatform2);
		Event* mvp1_coli_downwall = new collisionEvent(movingPlatform1, death_zone);
		Event* mvp2_coli_char = new collisionEvent(movingPlatform2, character);
		Event* mvp2_coli_upwall = new collisionEvent(movingPlatform2, staticPlatform2);
		Event* mvp2_coli_downwall = new collisionEvent(movingPlatform2, death_zone);
		Event* cloud_coli_left = new collisionEvent(cloud, staticPlatform1);
		Event* cloud_coli_right = new collisionEvent(cloud, side_boundary);

		Event* scriptOn = new scriptEvent();

		// walls
		sf::RectangleShape side_boundary_left = staticPlatform1.drawObject();
		sf::RectangleShape wall_2 = staticPlatform2.drawObject();
		
		// Ground
		sf::RectangleShape ground_1 = staticPlatform5.drawObject();

		// right side boundary
		sf::RectangleShape side_boundary_right = side_boundary.drawObject();

		// death_zone_1
		sf::RectangleShape death_zone_1 = death_zone.drawObject();

		// character
		sf::RectangleShape character_1 = character.drawObject();
		//character_1.setTexture(&tank);
		//character_1.setScale(1.5, 1.5);
		
		// Spawn Point 1
		sf::RectangleShape SpawnPoints_1 = SpawnPoints.drawObject();

		// moving platform
		sf::RectangleShape movePlatform_1 = movingPlatform1.drawObject();
		sf::RectangleShape movePlatform_2 = movingPlatform2.drawObject();
		sf::RectangleShape cloud_1 = cloud.drawObject();
		cloud_1.setTexture(&cloud_t);

		// level 2
		sf::RectangleShape ground_2 = staticPlatform6.drawObject();
		sf::RectangleShape ground_3 = staticPlatform7.drawObject();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (halfSpeed == 1) {
			if (eh_character->onEvent(key_Space)) { // sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)
				halfSpeed = 0;
				normalSpeed = 1;
				doubleSpeed = 0;
				scale = 0.5;
			}
		}
		else if (normalSpeed == 1) {
			if (eh_character->onEvent(key_Space)) {
				halfSpeed = 0;
				normalSpeed = 0;
				doubleSpeed = 1;
				scale = 1.0;
			}
		}
		else if (doubleSpeed == 1) {
			if (eh_character->onEvent(key_Space)) {
				halfSpeed = 1;
				normalSpeed = 0;
				doubleSpeed = 0;
				scale = 2.0;
			}
		}

		if (pause == true) {
			velocity = 0;
		}
		else {
			velocity = scale * step_size / fps;
		}

		// start/end recording
		if (eh_character->onEvent(key_S)) {
			isrecording = true;
			std::cout << "Start Recording!!!!" << std::endl;
			// record start position
			startRecPosition = character.drawObject().getPosition();
		}
		
		if (eh_character->onEvent(key_E)) {
			isrecording = false;
			std::cout << "End Recording!!!!" << std::endl;
		}

		// press M to enable scripting for character
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

		string s = "";
		if (eh_character->onEvent(key_Left)) { // sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)
			if (isrecording == true) {
				eventQueue.push_back(key_Left);

			}

			s = "l";
		}
		else if (eh_character->onEvent(key_Right)) { // sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)
			if (isrecording == true) {
				eventQueue.push_back(key_Right);
			}
			
			s = "r";
		}
		else if (eh_character->onEvent(key_Up)) { // sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)
			if (isrecording == true) {
				eventQueue.push_back(key_Up);
			}
			
			s = "u";
		}
		else if (eh_character->onEvent(key_Down)) { // sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)
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
			character.updateLeft(velocity);
		}
		else if (sub.compare("r") == 0) {
			character.updateRight(velocity);
		}
		else if (sub.compare("u") == 0) {
			character.updateUp(velocity);
		}
		else if (sub.compare("d") == 0) {
			if (eh_character->onEvent(coli_ground_1)) { // character_1.getGlobalBounds().intersects(ground_1.getGlobalBounds())
				//std::cout << "Collision!" << std::endl;
			}
			else if (eh_character->onEvent(coli_ground_2)) { // character_1.getGlobalBounds().intersects(ground_2.getGlobalBounds())
				//std::cout << "Collision!" << std::endl;
			}
			else if (eh_character->onEvent(coli_ground_3)) { // character_1.getGlobalBounds().intersects(ground_3.getGlobalBounds())
				//std::cout << "Collision!" << std::endl;
			}
			else {
				character.updateDown(velocity);
			}
		}
		else {
			if (eh_character->onEvent(char_death)) { // character_1.getGlobalBounds().intersects(death_zone_1.getGlobalBounds())
				//std::cout << character_1.getPosition().y << std::endl;
				std::cout << "You dead!" << std::endl;
				std::cout << "Start again at the spawn point!!" << std::endl;
			}
			if (eh_character->onEvent(coli_ground_1)) { // character_1.getGlobalBounds().intersects(ground_1.getGlobalBounds())
				//std::cout << "Collision!" << std::endl;
			}
			else if (eh_character->onEvent(coli_ground_2)) { // character_1.getGlobalBounds().intersects(ground_2.getGlobalBounds())
				//std::cout << "Collision!" << std::endl;
			}
			else if (eh_character->onEvent(coli_ground_3)) { // character_1.getGlobalBounds().intersects(ground_3.getGlobalBounds())
				//std::cout << "Collision!" << std::endl;
			}
			else if (eh_character->onEvent(coli_mvplatf_1)) { // character_1.getGlobalBounds().intersects(movePlatform_1.getGlobalBounds())
				//std::cout << "Collision!" << std::endl;
			}
			else if (eh_character->onEvent(coli_mvplatf_2)) { // character_1.getGlobalBounds().intersects(movePlatform_2.getGlobalBounds())
				//std::cout << "Collision!" << std::endl;
			}
			else if (!eh_character->onEvent(char_death)) {  // !character_1.getGlobalBounds().intersects(death_zone_1.getGlobalBounds())
				character.updateDown(velocity);
			}
			else {
				if (eh_character->onEvent(char_spawn)) {
					character.resetPosition(SpawnPoints_1.getPosition().x, SpawnPoints_1.getPosition().y);
				}
			}
		}

		// Moving platform1 start from up to down
		if (eh_platform->onEvent(mvp1_coli_upwall)) { // movePlatform_1.getGlobalBounds().intersects(wall_2.getGlobalBounds())
			upcoli = true;
			downcoli = false;
		}
		else if (eh_platform->onEvent(mvp1_coli_downwall)) { // movePlatform_1.getGlobalBounds().intersects(death_zone_1.getGlobalBounds())
			downcoli = true;
			upcoli = false;
		}
		if (upcoli == true) {
			movingPlatform1.updateDown(velocity);
			if (downcoli == true) {
				upcoli = false;
			}
		}
		else {
			if (eh_platform->onEvent(mvp1_coli_char)) { // movePlatform_1.getGlobalBounds().intersects(character_1.getGlobalBounds())
				//std::cout << "Collision!" << std::endl;
				character.updateUp(velocity);
			}
			else {
				movingPlatform1.updateUp(velocity);
			}
		}

		// Moving platform2 start from down to up
		if (eh_platform->onEvent(mvp2_coli_upwall)) { // movePlatform_2.getGlobalBounds().intersects(wall_2.getGlobalBounds())
			targetupcoli = true;
			targetdowncoli = false;
		}
		else if (eh_platform->onEvent(mvp2_coli_downwall)) { // movePlatform_2.getGlobalBounds().intersects(death_zone_1.getGlobalBounds())
			targetdowncoli = true;
			targetupcoli = false;
		}
		if (targetdowncoli == true) {
			if (eh_platform->onEvent(mvp2_coli_char)) { // movePlatform_2.getGlobalBounds().intersects(character_1.getGlobalBounds())
				//std::cout << "Collision!" << std::endl;
				character.updateUp(velocity);
			}
			else {
				movingPlatform2.updateUp(velocity);
			}
			if (targetupcoli == true) {
				targetdowncoli = false;
			}
		}
		else {
			movingPlatform2.updateDown(velocity);
		}

		// cloud move from left to right
		if (eh_platform->onEvent(cloud_coli_left)) { // cloud_1.getGlobalBounds().intersects(side_boundary_left.getGlobalBounds())
			targetleftcoli = true;
			targetrightcoli = false;
		}
		else if (eh_platform->onEvent(cloud_coli_right)) { // cloud_1.getGlobalBounds().intersects(side_boundary_right.getGlobalBounds())
			targetrightcoli = true;
			targetleftcoli = false;
		}
		if (targetleftcoli == true) {
			cloud.updateRight(4);
			if (targetrightcoli == true) {
				targetleftcoli = false;
			}
		}
		else {
			cloud.updateLeft(4);
		}

		if (eh_character->onEvent(coli_sideBound_right)) { // character_1.getGlobalBounds().intersects(side_boundary_right.getGlobalBounds())
			positionComponents p(-1000.f, -1000.f);
			positionComponents p2(300.f, 300.f);
			positionComponents p3(300.f, 270.f);
			positionComponents p4(0.f, 500.f);
			staticPlatform5.setPositionComponent(p);
			movingPlatform1.setPositionComponent(p);
			movingPlatform2.setPositionComponent(p);
			staticPlatform6.setPositionComponent(p2);
			staticPlatform7.setPositionComponent(p4);
			character.setPositionComponent(p3);
		}

		if (eh_character->onEvent(coli_sideBound_left)) { // character_1.getGlobalBounds().intersects(side_boundary_left.getGlobalBounds())
			staticPlatform5.setPositionComponent(groundPos);
			movingPlatform1.setPositionComponent(movingPlatformPos1);
			movingPlatform2.setPositionComponent(movingPlatformPos2);
			staticPlatform6.setPositionComponent(level2_Ground_pos);
			staticPlatform7.setPositionComponent(level2_Ground_pos);
			character.setPositionComponent(SpawnPos);
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

		// Event about replay
		if (eh_character->onEvent(key_R) && eventQueue.size() != 0) {
			std::cout << "Replaying!!!!" << std::endl;
			character.resetPosition(startRecPosition.x, startRecPosition.y);
			isReplay = true;
		}
		else if (isReplay && eventQueue.size() != 0) {
			for (int a = left_count; a > 0; a--) {
				character.updateLeft(velocity);
			}
			for (int b = right_count; b > 0; b--) {
				character.updateRight(velocity);
			}
			for (int c = up_count; c > 0; c--) {
				character.updateUp(velocity);
			}
			for (int d = down_count; d > 0; d--) {
				character.updateDown(velocity);
			}
			// remove all events after each replay
			eventQueue.clear();
			left_count = 0;
			right_count = 0;
			up_count = 0;
			down_count = 0;
			isReplay = false;
		}

		int deltaT = gt.getTime();
		//std::cout << deltaT << std::endl;
		window.clear();
		//window.draw(side_boundary_left);
		//window.draw(wall_2);
		//window.draw(side_boundary_right);
		//window.draw(death_zone_1);
		window.draw(ground_1);
		window.draw(ground_2);
		window.draw(ground_3);
		//std::cout << character_1.getPosition().x << " : " << character_1.getPosition().y << std::endl;
		window.draw(character_1);
		window.draw(movePlatform_1);
		window.draw(movePlatform_2);
		window.draw(cloud_1);
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
