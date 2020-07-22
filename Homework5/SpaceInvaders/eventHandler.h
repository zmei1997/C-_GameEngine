#pragma once
#include "Event.h"
class eventHandler
{
public:
	virtual bool onEvent(Event *e) = 0;
};

