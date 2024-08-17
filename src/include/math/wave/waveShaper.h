#include "math/mathFunctions.h"
struct waveShaper
{
    constexpr waveShaper(cfp &waveLength, cfp &minAmp, cfp &maxAmp, cfp &offset = 0) : waveLength(waveLength), minAmp(minAmp), maxAmp(maxAmp), offset(offset) {}
    // the length of a single wave
    fp waveLength = 0;

    // the minimum angle of a swing
    fp minAmp = 0;

    // the maximum angle of a swing
    fp maxAmp = 0;

    // offset in units
    fp offset = 0;
    
    fp getSineAmpAt(cfp &timePoint) const;
    fp getSawtoothAmpAt(cfp& timePoint) const;
    fp getSquareAmpAt(cfp& timePoint) const;
    bool maximumSineAmpBetween(cfp &timePoint0, cfp &timePoint1) const noexcept;
    void changeWaveLengthAt(cfp &changeOffset, cfp &newWaveLength);
};