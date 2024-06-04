#pragma once
/**

 * @file    SimplexNoise.h

 * @brief   A Perlin Simplex Noise C++ Implementation (1D, 2D, 3D).

 *

 * Copyright (c) 2014-2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

 *

 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt

 * or copy at http://opensource.org/licenses/MIT)

 */

#pragma once
#include <cstddef>  // int
#include <cstdint>  // int32_t/uint8_t
#include "vector\vectn.h"
#include "mathFunctions.h"
#include <random>

 //A Perlin Simplex Noise C++ Implementation (1D, 2D, 3D).
struct simplexNoise
{
public:
	static constexpr size_t noiseRepeat = 0x100;
	//functions:
	simplexNoise(std::mt19937& randomToUse);

	//variables:
/**

* Permutation table. This is just a random jumble of all numbers 0-255.

*

* This produce a repeatable pattern of 256, but Ken Perlin stated

* that it is not a problem for graphic texture as the noise features disappear

* at a distance far enough to be able to see a repeatable pattern of 256.

*

* This needs to be exactly the same for all instances on all platforms,

* so it's easiest to just keep it as static explicit data.

* This also removes the need for any initialisation of this class.

*

* Note that making this an uint32_t[] instead of a uint8_t[] might make the

* code run faster on platforms with a high penalty for unaligned single

* byte addressing. Intel x86 is generally single-byte-friendly, but

* some other CPUs are faster with 4-aligned reads.

* However, a char[] is smaller, which avoids cache trashing, and that

* is probably the most important aspect on most architectures.

* This array is accessed a *lot* by the noise functions.

* A vector-valued noise over 3D accesses it 96 times, and a

* fp-valued 4D noise 64 times. We want this to fit in the cache!

*/
	uint8_t perm[noiseRepeat];
	//functions:

/**

 * Helper function to hash an integer using the above permutation table

 *

 *  This inline function costs around 1ns, and is called N+1 times for a noise of N dimension.

 *

 *  Using a real hash function would be better to improve the "repeatability of 256" of the above permutation table,

 * but fast integer Hash functions uses more time and have bad random properties.

 *

 * @param[in] i Integer value to hash

 *

 * @return 8-bits hashed value

 */

	constexpr inline uint8_t hash(const int32_t& i) const
	{
		if constexpr (noiseRepeat == 0x100)
		{
			return perm[static_cast<uint8_t>(i)];
		}
		else 
		{
			return perm[i % noiseRepeat];
		}
	}

	//template<int dimensions>
	//static fp grad(const int32_t& hash, cvectn<fp, dimensions>& position)
	//{
	//	//throw("incorrect amount of dimensions");
	//	static_assert(false, "incorrect amount of dimensions (1 to 3 are allowed)");
	//}

	/**

 * Helper function to compute gradients-dot-residual vectors (1D)

 *

 * @note that these generate gradients of more than unit length. To make

 * a close match with the value range of classic Perlin noise, the final

 * noise values need to be rescaled to fit nicely within [-1,1].

 * (The simplex noise functions as such also have different scaling.)

 * Note also that these noise functions are the most practical and useful

 * signed version of Perlin noise.

 *

 * @param[in] hash  hash value

 * @param[in] x     distance to the corner

 *

 * @return gradient value

 */

	//template<>
	static fp grad(const int32_t& hash, cvectn<fp, 1>& position)
	{
		const int32_t& h = hash & 0x0F;  // Convert low 4 bits of hash code

		fp grad = (fp)1.0 + (fp)(h & 0b111);    // Gradient value 1.0, 2.0, ..., 8.0

		if ((h & 8) != 0) grad = -grad; // Set a random sign for the gradient

	//  fp grad = gradients1D[h];    // NOTE : Test of Gradient look-up table instead of the above

		return (grad * position.x);              // Multiply the gradient with the distance

	}

	/**

	 * Helper functions to compute gradients-dot-residual vectors (2D)

	 *

	 * @param[in] hash  hash value

	 * @param[in] x     x coord of the distance to the corner

	 * @param[in] y     y coord of the distance to the corner

	 *

	 * @return gradient value

	 */

	//template<>
	static fp grad(const int32_t& hash, cvectn<fp, 2>& position)
	{

		const int32_t h = hash & 0x3F;  // Convert low 3 bits of hash code

		cfp u = h < 4 ? position[0] : position[1];  // into 8 simple gradient directions,

		cfp v = h < 4 ? position[1] : position[0];

		return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v); // and compute the dot product with (x,y).

	}

	/**

	 * Helper functions to compute gradients-dot-residual vectors (3D)

	 *

	 * @param[in] hash  hash value

	 * @param[in] x     x coord of the distance to the corner

	 * @param[in] y     y coord of the distance to the corner

	 * @param[in] z     z coord of the distance to the corner

	 *

	 * @return gradient value

	 */

	//template<>
	static fp grad(const int32_t& hash, cvectn<fp, 3>& position)
	{

		cint& h = hash & 15;     // Convert low 4 bits of hash code into 12 simple

		cfp& u = h < 8 ? position.axis[0] : position.axis[1]; // gradient directions, and compute dot product.

		cfp& v = h < 4 ? position.axis[1] : h == 12 || h == 14 ? position.axis[0] : position.axis[2]; // Fix repeats at h = 12 to 15

		return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
	}



	//template<int dimensions>
	//inline fp evaluate(cvectn<fp, dimensions>& position) const
	//{
	//	//static_assert(false);
	//	throw "wrong number of dimensions";
	//}

	//1D Perlin simplex noise
	//Takes around 74ns on an AMD APU.
	//return: Noise value in the range[-1; 1], value of 0 on all integer coordinates.
	template<int dimensions>
	inline std::enable_if_t<dimensions == 1, fp> evaluate(cvectn<fp, dimensions>& position) const
	{
		fp n0, n1;   // Noise contributions from the two "corners"
	// No need to skew the input space in 1D

	// Corners coordinates (nearest integer values):

		const int32_t& i0 = math::floor(position.x);

		const int32_t& i1 = i0 + 1;

		// Distances to corners (between 0 and 1):

		cfp& x0 = position.x - i0;

		cfp& x1 = x0 - 1.0f;

		// Calculate the contribution from the first corner

		cfp& t0 = math::squared(1.0f - math::squared(x0));

		//  if(t0 < 0.0f) t0 = 0.0f; // not possible

		n0 = t0 * t0 * grad(hash(i0), cvectn<fp, 1>(x0));

		// Calculate the contribution from the second corner

		fp t1 = 1.0f - x1 * x1;

		//  if(t1 < 0.0f) t1 = 0.0f; // not possible

		t1 *= t1;

		n1 = t1 * t1 * grad(hash(i1), cvectn<fp, 1>(x1));

		// The maximum value of this noise is 8*(3/4)^4 = 2.53125
		// A factor of 0.395 scales to fit exactly within [-1,1]

		return 0.395f * (n0 + n1);
	}

	//2D Perlin simplex noise
//Takes around 150ns on an AMD APU.
//return: Noise value in the range[-1; 1], value of 0 on all integer coordinates.
	template<int dimensions>
	inline std::enable_if_t<dimensions == 2, fp> evaluate(cvectn<fp, dimensions>& position) const
	{

		fp n0, n1, n2;   // Noise contributions from the three corners

		// Skewing/Unskewing factors for 2D

		static cfp F2 = 0.366025403f;  // F2 = (sqrt(3) - 1) / 2

		static cfp G2 = 0.211324865f;  // G2 = (3 - sqrt(3)) / 6   = F2 / (1 + 2 * K)

		// Skew the input space to determine which simplex cell we're in

		cfp s = (position.x + position.y) * F2;  // Hairy factor for 2D

		cfp xs = position.x + s;

		cfp ys = position.y + s;

		const int32_t i = math::floor(xs);

		const int32_t j = math::floor(ys);

		// Unskew the cell origin back to (x,y) space

		cfp t = static_cast<fp>(i + j) * G2;

		cfp X0 = i - t;

		cfp Y0 = j - t;

		cfp x0 = position.x - X0;  // The x,y distances from the cell origin

		cfp y0 = position.y - Y0;

		// For the 2D case, the simplex shape is an equilateral triangle.

		// Determine which simplex we are in.

		int32_t i1, j1;  // Offsets for second (middle) corner of simplex in (i,j) coords

		if (x0 > y0) {   // lower triangle, XY order: (0,0)->(1,0)->(1,1)
			i1 = 1;
			j1 = 0;
		}
		else
		{   // upper triangle, YX order: (0,0)->(0,1)->(1,1)
			i1 = 0;
			j1 = 1;
		}

		// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and

		// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where

		// c = (3-sqrt(3))/6

		cfp x1 = x0 - i1 + G2;            // Offsets for middle corner in (x,y) unskewed coords

		cfp y1 = y0 - j1 + G2;

		cfp x2 = x0 - 1.0f + 2.0f * G2;   // Offsets for last corner in (x,y) unskewed coords

		cfp y2 = y0 - 1.0f + 2.0f * G2;

		// Work out the hashed gradient indices of the three simplex corners

		cint gi0 = hash(i + hash(j));

		cint gi1 = hash(i + i1 + hash(j + j1));

		cint gi2 = hash(i + 1 + hash(j + 1));

		// Calculate the contribution from the first corner

		fp t0 = 0.5f - x0 * x0 - y0 * y0;

		if (t0 < 0.0f) {
			n0 = 0.0f;
		}
		else
		{
			t0 *= t0;
			n0 = t0 * t0 * grad(gi0, cvectn<fp, 2>(x0, y0));
		}

		// Calculate the contribution from the second corner

		fp t1 = 0.5f - x1 * x1 - y1 * y1;

		if (t1 < 0.0f)
		{
			n1 = 0.0f;
		}
		else
		{
			t1 *= t1;
			n1 = t1 * t1 * grad(gi1, cvectn<fp, 2>(x1, y1));
		}

		// Calculate the contribution from the third corner

		fp t2 = 0.5f - x2 * x2 - y2 * y2;

		if (t2 < 0.0f) {

			n2 = 0.0f;

		}
		else
		{

			t2 *= t2;

			n2 = t2 * t2 * grad(gi2, cvectn<fp, 2>(x2, y2));

		}

		// Add contributions from each corner to get the final noise value.

		// The result is scaled to return values in the interval [-1,1].

		return 45.23065f * (n0 + n1 + n2);

	}

	//3D Perlin simplex noise
	//return: Noise value in the range[-1; 1], value of 0 on all integer coordinates.

	template<int dimensions>
	inline std::enable_if_t<dimensions == 3, fp> evaluate(cvectn<fp, dimensions>& position) const
	{
		fp n0, n1, n2, n3; // Noise contributions from the four corners

		// Skewing/Unskewing factors for 3D

		constexpr fp F3 = 1.0f / 3.0f;

		constexpr fp G3 = 1.0f / 6.0f;

		// Skew the input space to determine which simplex cell we're in

		cfp s = (position.sum()) * F3; // Very nice and simple skew factor for 3D

		cint i = math::floor(position.x + s);

		cint j = math::floor(position.y + s);

		cint k = math::floor(position.z + s);

		cfp t = (i + j + k) * G3;

		cfp X0 = i - t; // Unskew the cell origin back to (x,y,z) space

		cfp Y0 = j - t;

		cfp Z0 = k - t;

		cfp x0 = position.x - X0; // The x,y,z distances from the cell origin

		cfp y0 = position.y - Y0;

		cfp z0 = position.z - Z0;

		// For the 3D case, the simplex shape is a slightly irregular tetrahedron.

		// Determine which simplex we are in.

		int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords

		int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords

		if (x0 >= y0) {

			if (y0 >= z0) {

				i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0; // X Y Z order

			}
			else if (x0 >= z0) {

				i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; // X Z Y order

			}
			else
			{

				i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; // Z X Y order

			}

		}
		else
		{ // x0<y0

			if (y0 < z0) {

				i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; // Z Y X order

			}
			else if (x0 < z0) {

				i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; // Y Z X order

			}
			else
			{

				i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; // Y X Z order

			}

		}

		// A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),

		// a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and

		// a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where

		// c = 1/6.

		cfp x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords

		cfp y1 = y0 - j1 + G3;

		cfp z1 = z0 - k1 + G3;

		cfp x2 = x0 - i2 + 2.0f * G3; // Offsets for third corner in (x,y,z) coords

		cfp y2 = y0 - j2 + 2.0f * G3;

		cfp z2 = z0 - k2 + 2.0f * G3;

		cfp x3 = x0 - 1.0f + 3.0f * G3; // Offsets for last corner in (x,y,z) coords

		cfp y3 = y0 - 1.0f + 3.0f * G3;

		cfp z3 = z0 - 1.0f + 3.0f * G3;

		// Work out the hashed gradient indices of the four simplex corners

		cint gi0 = hash(i + hash(j + hash(k)));

		cint gi1 = hash(i + i1 + hash(j + j1 + hash(k + k1)));

		cint gi2 = hash(i + i2 + hash(j + j2 + hash(k + k2)));

		cint gi3 = hash(i + 1 + hash(j + 1 + hash(k + 1)));

		// Calculate the contribution from the four corners

		fp t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;

		if (t0 < 0) {

			n0 = 0.0;

		}
		else
		{

			t0 *= t0;

			n0 = t0 * t0 * grad(gi0, cvectn<fp, 3>(x0, y0, z0));

		}

		fp t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;

		if (t1 < 0) {

			n1 = 0.0;

		}
		else
		{

			t1 *= t1;

			n1 = t1 * t1 * grad(gi1, cvectn<fp, 3>(x1, y1, z1));

		}

		fp t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;

		if (t2 < 0) {

			n2 = 0.0;

		}
		else
		{

			t2 *= t2;

			n2 = t2 * t2 * grad(gi2, cvectn<fp, 3>(x2, y2, z2));

		}

		fp t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;

		if (t3 < 0) {

			n3 = 0.0;

		}
		else
		{

			t3 *= t3;

			n3 = t3 * t3 * grad(gi3, cvectn<fp, 3>(x3, y3, z3));

		}

		// Add contributions from each corner to get the final noise value.

		// The result is scaled to stay just inside [-1,1]

		return 32.0f * (n0 + n1 + n2 + n3);

	}
};