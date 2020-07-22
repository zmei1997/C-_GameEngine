#pragma once
#include "eventHandler.h"
#include "spawnEvent.h"

class charEventHandler : public eventHandler
{
public:
	charEventHandler();
	bool onEvent(Event *e);
};

