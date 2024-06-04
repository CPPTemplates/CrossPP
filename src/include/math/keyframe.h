#pragma once
#include "globalFunctions.h"
#include "interface/idestructable.h"
#include "mathFunctions.h"
#include <vector>

template<typename t>
struct keyFrame
{
	constexpr keyFrame(cfp& location, const t& value)
	{
		this->location = location;
		this->value = value;
	}
	fp location;
	t value;
};
template<typename t>
struct transition :IDestructable
{
	//has to be sorted from least to most
	std::vector<keyFrame<t>> keyframes;

	constexpr transition(const std::vector<keyFrame<t>>& keyframes)
	{
		this->keyframes = keyframes;
	}

	constexpr t getValue(cfp& location) const
	{
		cfsize_t& KeyframeCount = (fsize_t)keyframes.size();
		if (keyframes[0].location >= location)
		{
			return keyframes[0].value;
		}
		//interpolate from i - 1 to i
		for (size_t i = 1; i < keyframes.size(); i++)
		{
			const keyFrame<t>& currentkeyframe = keyframes[i];
			if (currentkeyframe.location >= location)
			{
				const keyFrame<t>& lastkeyframe = keyframes[i - 1];
				//interpolate
				cfp& weight = math::getw(lastkeyframe.location, currentkeyframe.location, location);
				return math::lerp(lastkeyframe.value, currentkeyframe.value, weight);
			}
		}
		return keyframes[KeyframeCount - 1].value;
	}
};
