#include "dimension.h"
#include "blaze.h"
#include "taskList.h"
#include "blazeFireBallAI.h"
#include "attackOnCollideAI.h"
#include "wanderAI.h"
#include "world.h"
blaze::blaze(dimension *dimensionIn, cvec2 &position) : mob(dimensionIn, position, entityID::blaze)
{
	tasks = new taskList(this, {
								   new blazeFireBallAI(this),
								   new attackOnCollideAI(this, entityID::human),
								   new wanderAI(this),
							   });

	flying = true;

	head = new bodyPart2D(blazeHeadTextureRect, nullptr, cvec2(), blazeHeadSize, blazeHeadSize * 0.5);
	mainBodyPart = head;
	for (size_t i = 0; i < blazeRodCount; i++)
	{
		rods[i] = new bodyPart2D(blazeRodTextureRect, head, cvec2(), blazeRodSize, cvec2(blazeRodSize.x * 0.5, blazeRodSize.y));
		head->children.push_back(rods[i]);
	}
	head->drawOrderIndex = (blazeRingCount * (blazeRingRodCount / 2));
}

void blaze::updateBodyParts() const
{
	head->translate = position + cvec2(0, blazeHitbox.size.y - blazeHeadSize.y * 0.5);
	head->changed = true;
	for (size_t ringIndex = 0; ringIndex < blazeRingCount; ringIndex++)
	{
		// the rods don't rotate really; they go forward until a quarter and then lag back, in order to maintain the correct z-order
		cfp additionalRingRotation = math::mod((fp)(currentWorld->ticksSinceStart * secondsPerTick * blazeRodSpeed[ringIndex]), (fp)(1.0 / blazeRingRodCount));
		for (size_t ringRodIndex = 0; ringRodIndex < blazeRingRodCount; ringRodIndex++)
		{
			// group 0: behind the head
			// group 1: in front of the head
			csize_t &rodIndex =
				(ringRodIndex < blazeRingBackRodCount) ? ringIndex * blazeRingBackRodCount + ringRodIndex : (ringIndex + blazeRingCount) * blazeRingBackRodCount + (ringRodIndex - blazeRingBackRodCount);

			cfp rodRotation = (((fp)ringRodIndex / (fp)blazeRingRodCount) + additionalRingRotation) * math::PI2;
			rods[rodIndex]->translate = cvec2(-cos(rodRotation) * blazeRingRadius[ringIndex], relativeBlazeRingHeight[ringIndex]);
			rods[rodIndex]->changed = true;
		}
	}
}