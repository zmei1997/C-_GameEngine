#include <SFML/Graphics.hpp>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include "GameObject.h"
#include "positionComponents.h"
#include "colorComponents.h"
#include "sizeComponents.h"
#include "movementComponents.h"

using namespace std;

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
bool unpause = true;
int scale = 1;
int oneScale = 1;
int twiceScele = 0;
int fourScale = 0;

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

static const int numberOfThreads = 1;
std::string c = "";
std::string response = "";

void server_thread(string c) {
	if (c == "l") {
		response = "l";
	}
	else if (c == "r") {
		response = "r";
	}
	else if (c == "u") {
		response = "u";
	}
	else if (c == "d") {
		response = "d";
	}
}

int main() {
	
	thread t[numberOfThreads];

	zmq::context_t context(1);
	zmq::socket_t publisher(context, ZMQ_PUB);
	zmq::socket_t server(context, ZMQ_REP);
	publisher.bind("tcp://*:5563");
	server.bind("tcp://*:5564");
	std::string str = s_recv(server);
	s_send(server, "");

	while (1) {
		
		for (int i = 0; i < numberOfThreads; i++) {
			c = s_recv(server);
			t[i] = thread(server_thread, c);
			s_send(server, c);
		}

		for (int i = 0; i < numberOfThreads; i++) {
			t[i].join();
		}

		s_sendmore(publisher, "1");
		//s_send(publisher, response);
		s_send(publisher, c);
	}

	return 0;
}