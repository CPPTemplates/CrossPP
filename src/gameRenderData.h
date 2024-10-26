#pragma once
#include "renderData.h"
#include "globalFunctions.h"
#include "math/timemath.h"
#include "constants.h"
struct gameControl;
struct gameRenderData : public renderData
{
	const gameControl &screen;
	fp secondsOffset;
	fp tickPartOffset;
	gameRenderData(cmat3x3 &worldToRenderTargetTransform, const texture &renderTarget, const gameControl &screen, const fp &secondsOffset = 0)
		: renderData(worldToRenderTargetTransform, renderTarget),
		  screen(screen),
		  secondsOffset(secondsOffset), tickPartOffset(secondsOffset * ticksPerRealLifeSecond)

	{
	}

	constexpr vec2 getRenderOffset(cvec2 &oldPosition, cvec2 &newPosition) const
	{
		return (newPosition - oldPosition) * tickPartOffset;
	}
	constexpr vec2 getRenderPosition(cvec2 &oldPosition, cvec2 &newPosition) const
	{
		return oldPosition + getRenderOffset(oldPosition, newPosition);
	}

	constexpr fp getSecondsSinceStart(cint &ticksSinceStart) const
	{
		return ((ticksSinceStart + tickPartOffset) / ticksPerRealLifeSecond);
	}

	gameRenderData clone(cmat3x3 &newMatrix) const
	{
		return gameRenderData(newMatrix, renderTarget, screen, secondsOffset);
	}
};