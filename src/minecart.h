#pragma once
#include "hollowRidableEntity.h"
//minecart
constexpr rectangle2 minecartBottomTextureRect = crectangle2(24, 20, 16, 2);
constexpr rectangle2 minecartTopTextureRect = crectangle2(20, 22, 16, 8); 
constexpr rectangle2 minecartEditedTextureRect = crectangle2(0, 0, minecartBottomTextureRect.size.getX(), minecartBottomTextureRect.size.getY() + minecartTopTextureRect.size.getY());
constexpr int minecartWheelPixelSize = 6;

constexpr vec2 minecartHitboxSize = cvec2(1, 0.7);

constexpr fp minecartVolume = minecartHitboxSize.volume() * minecartHitboxSize.getX();
constexpr fp minecartWeight = minecartVolume * ironWeightPerCubicMeter;

constexpr fp minecartPixelSize = minecartHitboxSize.getX() / minecartEditedTextureRect.size.getX();

constexpr vec2 minecartRenderSize = cvec2(minecartEditedTextureRect.size) * minecartPixelSize;
constexpr vec2 minecartRotationCentre = cvec2(minecartRenderSize.getX() * 0.5, 0);

constexpr fp minecartWheelSize = minecartWheelPixelSize * minecartPixelSize;
struct minecart : public hollowRidableEntity
{
	minecart(dimension* dimensionIn, cvec2& position);
	virtual void render(const gameRenderData& targetData) const override;
	virtual void tick() override;
	virtual void onDeath() override;
	virtual vec3 getGroundFriction() const override;
	virtual vec2 getRelativeSeatPosition(csize_t& seatIndex) const override;
	const vect2<blockID> getBlockWheelsOn() const;
	const vect2<veci2> getwheelPositions() const;
	virtual fp getLengthTouchingGround() const override;
	virtual fp getHollowVolume() const final;
	virtual fp getWeightPerCubicMeter() const final;
};