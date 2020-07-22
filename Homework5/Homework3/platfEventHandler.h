#pragma once
#include "eventHandler.h"

class platfEventHandler : public eventHandler
{
public:
	platfEventHandler();
	bool onEvent(Event* e);
};

