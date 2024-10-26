#include "pressurePlateData.h"
#include "world.h"
#include "soundList.h"
#include "tickableBlockContainer.h"
#include "entity.h"
#include "nbt/serializeUUID.h"
void pressurePlateData::addEntityStandingOn(tickableBlockContainer *containerIn, cveci2 &position, entity *const &e)
{
	if (lastUpdateTick < currentWorld->ticksSinceStart)
	{
		if (((lastUpdateTick + 1) < currentWorld->ticksSinceStart) || (entitiesStandingOn.size() == 0))
		{
			clickSound->playRandomSound(containerIn, position + cvec2(0.5, 0));
		}
		entitiesStandingOn = std::vector<uuid>({e->identifier});
		lastUpdateTick = currentWorld->ticksSinceStart;
	}
	else
	{
		if (std::find(entitiesStandingOn.begin(), entitiesStandingOn.end(), e->identifier) == entitiesStandingOn.end())
		{
			entitiesStandingOn.push_back(e->identifier);
		}
	}
	containerIn->addUpdatePosition(position);
}

bool pressurePlateData::tick(tickableBlockContainer *containerIn, cveci2 &position)
{
	if (currentWorld->ticksSinceStart == lastUpdateTick)
	{
		return true;
	}
	else if (currentWorld->ticksSinceStart == (lastUpdateTick + 1))
	{
		containerIn->addUpdatePosition(position);
	}
	return false;
}

void pressurePlateData::serializeValue(nbtSerializer &s)
{
	s.serializeValue(std::wstring(L"last update tick"), lastUpdateTick);
	if (lastUpdateTick != -1)
	{
		if (s.push<nbtDataTag::tagList>(std::wstring(L"entities")))
		{
			if (s.write)
			{
				for (uuid &entityStandingOn : entitiesStandingOn)
				{
					if (s.push<nbtDataTag::tagCompound>())
					{
						serializeNBTValue(s, std::wstring(L"uuid"), entityStandingOn);
						s.pop();
					}
				}
			}
			else
			{
				entitiesStandingOn.resize(s.getChildren().size());
				size_t i = 0;
				while (s.push<nbtDataTag::tagCompound>())
				{
					serializeNBTValue(s, std::wstring(L"uuid"), entitiesStandingOn[i]);
					s.pop();
					i++;
				}
			}
			s.pop();
		}
	}
}