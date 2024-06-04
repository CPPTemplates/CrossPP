#include "sound.h"
#include <cmath>
fp getNotePitch(cfp& relativeNote)
{
	return pow(2, relativeNote / octaveNoteCount);
}
