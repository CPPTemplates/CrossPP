#include "passiveMob.h"
#include "panicAI.h"
#include "wanderAI.h"
#include "taskList.h"
passiveMob::passiveMob(dimension *dimensionIn, cvec2 &position, const entityID &entityType) : mob(dimensionIn, position, entityType)
{
	this->tasks = new taskList(this, {
										 new panicAI(this),
										 new wanderAI(this),
									 });
}