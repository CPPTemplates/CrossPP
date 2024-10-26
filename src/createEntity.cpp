#include "zombifiedPiglin.h"
#include "enderDragon.h"
#include "zombie.h"
#include "skeleton.h"
#include "fluidList.h"
#include "entityData.h"
#include "idList.h"
#include "magmaCube.h"
#include "blaze.h"
#include "ghast.h"
#include "experienceOrb.h"
#include "sheep.h"
#include "creeper.h"
#include "pig.h"
#include "cow.h"
#include "wolf.h"
#include "cod.h"
#include "chicken.h"
#include "shulker.h"
#include "enderman.h"
#include "smallFireBall.h"
#include "ghastFireBall.h"
#include "dragonFireBall.h"
#include "boat.h"
#include "floatingSlot.h"
#include "egg.h"
#include "snowBall.h"
#include "enderPearl.h"
#include "eyeOfEnder.h"
#include "fishingHook.h"
#include "arrow.h"
#include "tnt.h"
#include "endCrystal.h"
#include "fallingBlock.h"
#include "minecart.h"
#include "particle.h"
#include "piston.h"
#include "fireworkRocket.h"
#include "pollen.h"
#include "slime.h"

#include "idAnalysis.h"
#include "world.h"

entity* createEntity(const entityID& entityType, dimension* dimensionIn, cvec2& position)
{
	switch (entityType)
	{
	case entityID::zombiefied_piglin:
		return new zombifiedPiglin(dimensionIn, position);
	case entityID::enderman:
		return new enderman(dimensionIn, position);
	case entityID::slime:
		return new slime(dimensionIn, position);
	case entityID::magma_cube:
		return new magmaCube(dimensionIn, position);
	case entityID::creeper:
		return new creeper(dimensionIn, position);
	case entityID::pig:
		return new pig(dimensionIn, position);
	case entityID::cow:
		return new cow(dimensionIn, position);
	case entityID::sheep:
		return new sheep(dimensionIn, position);
	case entityID::wolf:
		return new wolf(dimensionIn, position);
	case entityID::cod:
		return new cod(dimensionIn, position);
	case entityID::boat:
		return new boat(dimensionIn, position);
	case entityID::experience_orb:
		return new experienceOrb(dimensionIn, position);
	case entityID::item:
		return new floatingSlot(dimensionIn, position);
	case entityID::egg:
		return new egg(dimensionIn, position);
	case entityID::snowball:
		return new snowBall(dimensionIn, position);
	case entityID::ender_pearl:
		return new enderPearl(dimensionIn, position);
	case entityID::ender_eye:
		return new eyeOfEnder(dimensionIn, position);
	case entityID::fishing_bobber:
		return new fishingHook(dimensionIn, position);
	case entityID::arrow:
		return new arrow(dimensionIn, position);
	case entityID::ghast_fireball:
		return new ghastFireBall(dimensionIn, position);
	case entityID::dragon_fireball:
		return new dragonFireBall(dimensionIn, position);
	case entityID::small_Fireball:
		return new smallFireBall(dimensionIn, position);
	case entityID::tnt:
		return new tnt(dimensionIn, position);
	case entityID::end_crystal:
		return new endCrystal(dimensionIn, position);
	case entityID::falling_block:
		return new fallingBlock(dimensionIn, position);
	case entityID::shulker:
		return new shulker(dimensionIn, position);
	case entityID::blaze:
		return new blaze(dimensionIn, position);
	case entityID::ghast:
		return new ghast(dimensionIn, position);
	case entityID::ender_dragon:
		return new enderDragon(dimensionIn, position);
	case entityID::chicken:
		return new chicken(dimensionIn, position);
	case entityID::minecart:
		return new minecart(dimensionIn, position);
	case entityID::particle:
		return new particle(dimensionIn, position);
	case entityID::piston:
		return new piston(dimensionIn, position);
	case entityID::firework_rocket:
		return new fireworkRocket(dimensionIn, position);
	case entityID::pollen:
		return new pollen(dimensionIn, position);
	default:
		if (isZombie(entityType))
		{
			return new zombie(dimensionIn, position, entityType);
		}
		else if (isSkeleton(entityType))
		{
			return new skeleton(dimensionIn, position, entityType);
		}
		else
		{
			currentWorld->currentChat.addLine(std::wstring(L"summoning of this entity is not implemented"));
			return nullptr;
		}
		break;
	}
}