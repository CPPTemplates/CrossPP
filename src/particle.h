#pragma once
#include "entity.h"
#include "particleBrush.h"
//particle
constexpr fp particleSize = 0.25;
constexpr rectangle2 relativeParticleHitbox = crectangle2(vec2(particleSize * -0.5, 0), vec2(particleSize));
struct particle :public entity
{
	int ticksToExist = 0;

	particleBrush* brush = nullptr;
	particle(dimension* dimensionIn, cvec2& position);
	void tick() override;
	virtual void render(const gameRenderData& targetData) const override;
	virtual void serializeValue(nbtSerializer& s) override;
	virtual ~particle() override;
};


particle* summonParticle(tickableBlockContainer* containerIn, cvec2& position, particleBrush* linkedBrush, cfp& maxSpeed = maxParticleSpeed);