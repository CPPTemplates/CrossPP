#pragma once
#include <cstdint>
//https://stackoverflow.com/questions/919612/mapping-two-integers-to-one-in-a-unique-and-deterministic-way
//maps two int32_t's perfectly to one int64_t, does the same for smaller types
//(0,0) => 0 (1, 1) => 1
inline int64_t perfectlyHashToDoubleSizedType(const int32_t& a, const int32_t& b)
{
	uint64_t A = (a >= 0) ? (2 * (long)a) : (-2 * (long)a - 1);
	uint64_t B = (b >= 0) ? (2 * (long)b) : (-2 * (long)b - 1);
	int64_t C = (int64_t)(((A >= B) ? ((A * A) + A + B) : (A + (B * B))) / 2);
	return (a < 0 && b < 0) || (a >= 0 && b >= 0) ? C : (-C - 1);
}
