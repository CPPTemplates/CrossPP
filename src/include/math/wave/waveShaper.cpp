#include "waveShaper.h"

fp waveShaper::getSawtoothAmpAt(cfp &timePoint) const
{
	return minAmp + math::mod((timePoint - offset) / waveLength, (fp)1) * (maxAmp - minAmp);
}

fp waveShaper::getSquareAmpAt(cfp &timePoint) const
{
	return math::mod((timePoint - offset) / waveLength, (fp)1) >= 0.5 ? maxAmp : minAmp;
}

fp waveShaper::getSineAmpAt(cfp &timePoint) const
{
	return math::mapValue(sin(((timePoint - offset) / waveLength) * math::PI2), (fp)-1, (fp)1, minAmp, maxAmp);
}

bool waveShaper::maximumSineAmpBetween(cfp &timePoint0, cfp &timePoint1) const noexcept
{
	return floor((timePoint0 - offset) / waveLength) != floor((timePoint1 - offset) / waveLength);
}

void waveShaper::changeWaveLengthAt(cfp &changeOffset, cfp &newWaveLength)
{
	if (waveLength == newWaveLength)
		return;
	if (waveLength && newWaveLength)
	{
		cfp &difference = (changeOffset - offset);
		cfp &oldSwings = difference / waveLength;
		cfp &newOffsetFromchangeOffset = oldSwings * newWaveLength;
		this->offset = changeOffset - newOffsetFromchangeOffset;
		this->offset = math::mod(this->offset, newWaveLength);
	}
	this->waveLength = newWaveLength;
}