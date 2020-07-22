#include "eventManager.h"

eventManager::eventManager() {}

vector<Event*> eventManager::getQueue() {
	return evt;
}