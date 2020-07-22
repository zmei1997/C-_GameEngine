#pragma once
#include <vector>
#include "Event.h"

using namespace std;
class eventManager
{
private:
	vector<Event*> evt;
public:
	eventManager();
	vector<Event*> getQueue();
};

