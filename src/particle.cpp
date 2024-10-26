#include "math/vector/vectn.h"
#include "particleBrush.h"
#include "particle.h"
#include "dimension.h"
#include "blockParticleBrush.h"
#include "itemParticleBrush.h"
#include "include/math/vector/vectorrandom.h"
particle* summonParticle(tickableBlockContainer* containerIn, cvec2& position, particleBrush* linkedBrush, cfp& maxSpeed)
{
	particle* p = (particle*)summonEntity(entityID::particle, containerIn, position);
	p->brush = linkedBrush;
	p->speed = getRandomVector(currentRandom, maxSpeed);
	p->ticksToExist = particleDespawnTicks;
	return p;
}
particle::particle(dimension* dimensionIn, cvec2& position) :entity(dimensionIn, position, entityID::particle)
{
}

void particle::tick()
{
	ticksToExist--;
	if (ticksToExist <= 0)
	{
		despawn = true;
	}
	entity::tick();
}

void particle::render(const gameRenderData& targetData) const
{
	this->brush->render(calculateHitBox(), targetData);
}

void particle::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"ticks to exist"), ticksToExist);
	if (s.write)
	{
		s.serializeValue(std::wstring(L"particle type"), brush->particleType);

		brush->serializeValue(s);
	}
	else
	{
		particleID particleType = (particleID)0;
		if (s.serializeValue(std::wstring(L"particle type"), particleType))
		{
			switch (particleType)
			{
			case particleID::ambient_entity_effect:
				break;
			case particleID::angry_villager:
				break;
			case particleID::ash:
				break;
			case particleID::barrier:
				break;
			case particleID::block:
				brush = new blockParticleBrush();
				break;
			case particleID::bubble:
				break;
			case particleID::bubble_pop:
				break;
			case particleID::bubble_column_up:
				break;
			case particleID::campfire_cosy_smoke:
				break;
			case particleID::campfire_signal_smoke:
				break;
			case particleID::cloud:
				break;
			case particleID::composter:
				break;
			case particleID::crimson_spore:
				break;
			case particleID::crit:
				break;
			case particleID::current_down:
				break;
			case particleID::damage_indicator:
				break;
			case particleID::dolphin:
				break;
			case particleID::dragon_breath:
				break;
			case particleID::dripping_lava:
				break;
			case particleID::dripping_obsidian_tear:
				break;
			case particleID::dripping_water:
				break;
			case particleID::dust:
				break;
			case particleID::dust_color_transition:
				break;
			case particleID::effect:
				break;
			case particleID::elder_guardian:
				break;
			case particleID::enchant:
				break;
			case particleID::enchanted_hit:
				break;
			case particleID::end_rod:
				break;
			case particleID::entity_effect:
				break;
			case particleID::explosion_emitter:
				break;
			case particleID::explosion:
				break;
			case particleID::falling_dust:
				break;
			case particleID::falling_lava:
				break;
			case particleID::falling_obsidian_tear:
				break;
			case particleID::falling_water:
				break;
			case particleID::firework:
				break;
			case particleID::fishing:
				break;
			case particleID::flame:
				break;
			case particleID::flash:
				break;
			case particleID::happy_villager:
				break;
			case particleID::heart:
				break;
			case particleID::instant_effect:
				break;
			case particleID::item:
				brush = new itemParticleBrush();
				break;
			case particleID::item_slime:
				break;
			case particleID::item_snowball:
				break;
			case particleID::landing_lava:
				break;
			case particleID::landing_obsidian_tear:
				break;
			case particleID::large_smoke:
				break;
			case particleID::lava:
				break;
			case particleID::mycelium:
				break;
			case particleID::nautilus:
				break;
			case particleID::note:
				break;
			case particleID::poof:
				break;
			case particleID::portal:
				break;
			case particleID::rain:
				break;
			case particleID::smoke:
				break;
			case particleID::sneeze:
				break;
			case particleID::soul:
				break;
			case particleID::soul_fire_flame:
				break;
			case particleID::spit:
				break;
			case particleID::splash:
				break;
			case particleID::squid_ink:
				break;
			case particleID::sweep_attack:
				break;
			case particleID::totem_of_undying:
				break;
			case particleID::underwater:
				break;
			case particleID::vibration:
				break;
			case particleID::warped_spore:
				break;
			case particleID::witch:
				break;
			default:
				break;
			}
			if (brush)
			{
				brush->serializeValue(s);
			}
			else
			{
				handleError(std::wstring(L"invalid brush type"));
			}
		}
	}
}

particle::~particle()
{
	delete brush;
}
rectangle2 getParticleBrushRect(const resolutionTexture& tex)
{
	cfp texturePart = 0.2 * tex.defaultSize.x;
	cvec2 location = vec2(
		randFp(currentRandom, tex.defaultSize.x - texturePart),
		randFp(currentRandom, tex.defaultSize.x - texturePart)
	);
	return rectangle2(location, vec2(texturePart));
}