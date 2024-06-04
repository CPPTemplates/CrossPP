/**

 * @file    SimplexNoise.cpp

 * @brief   A Perlin Simplex Noise C++ Implementation (1D, 2D, 3D).

 *

 * Copyright (c) 2014-2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

 *

 * This C++ implementation is based on the speed-improved Java version 2012-03-09

 * by Stefan Gustavson (original Java source code in the public domain).

 * http://webstaff.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java:

 * - Based on example code by Stefan Gustavson (stegu@itn.liu.se).

 * - Optimisations by Peter Eastman (peastman@drizzle.stanford.edu).

 * - Better rank ordering method by Stefan Gustavson in 2012.

 *

 * This implementation is "Simplex Noise" as presented by

 * Ken Perlin at a relatively obscure and not often cited course

 * session "Real-Time Shading" at Siggraph 2001 (before real

 * time shading actually took on), under the title "hardware noise".

 * The 3D function is numerically equivalent to his Java reference

 * code available in the PDF course notes, although I re-implemented

 * it from scratch to get more readable code. The 1D, 2D and 4D cases

 * were implemented from scratch by me from Ken Perlin's text.

 *

 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt

 * or copy at http://opensource.org/licenses/MIT)

 */

//source:
//https://github.com/SRombauts/SimplexNoise/blob/master/src/SimplexNoise.cpp

#include "SimplexNoise.h"
//random
#include <stdlib.h>

 /**

  * Computes the largest integer value not greater than the fp one

  *

  * This method is faster than using (int32_t)std::floor(fp).

  *

  * I measured it to be approximately twice as fast:

  *  fp:  ~18.4ns instead of ~39.6ns on an AMD APU),

  *  fp: ~20.6ns instead of ~36.6ns on an AMD APU),

  * Reference: http://www.codeproject.com/Tips/700780/Fast-floor-ceiling-functions

  *

  * @param[in] fp    fp input value

  *

  * @return largest integer value not greater than fp

  */

/*inline static int32_t fastfloor(fp fpingpoint) {

	int32_t i = static_cast<int32_t>(fpingpoint);

	return (fpingpoint < i) ? (i - 1) : (i);

}*/

simplexNoise::simplexNoise(std::mt19937& randomToUse)
{
	//fill lookup table
	for (fsize_t i = 0; i < noiseRepeat; i++)
	{
		perm[i] = static_cast<uint8_t> (i);
	}
	//shuffle array
	//source:
	//http://www.cplusplus.com/reference/algorithm/shuffle/
	for (fsize_t i = noiseRepeat - 1; i > 0; i--)
	{
		const uint8_t& current = perm[i];
		cfsize_t& index = randomToUse() % (i + 1);
		perm[i] = perm[index];
		perm[index] = current;
	}
}



/* NOTE Gradient table to test if lookup-table are more efficient than calculs

static cfp gradients1D[16] = {

		-8.f, -7.f, -6.f, -5.f, -4.f, -3.f, -2.f, -1.f,

		 1.f,  2.f,  3.f,  4.f,  5.f,  6.f,  7.f,  8.f

};

*/

