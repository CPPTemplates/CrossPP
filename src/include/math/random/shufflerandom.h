#pragma once
#include <cstdint>
#include "math/algorithm/modInverse.h"
#include <bitset>

template <typename t>
struct shuffleRandom
{
	static constexpr size_t bitCount = sizeof(t) * 0x8;
	typedef long long signedBiggerType;
	static constexpr signedBiggerType maxPlus1 = (signedBiggerType)1 << (signedBiggerType)bitCount;

	t seed;
	static constexpr t randomPrimes[4]{
		(bitCount == 8) ? 0b10011101 : ((bitCount == 0x10) ? 0b1001001110110101 : ((bitCount == 0x20) ? 0b10000100011101011010001101010001 : 0b1111010010011000101100100110110011100110110110011011101001001001)),
		(bitCount == 8) ? 0b11010011 : ((bitCount == 0x10) ? 0b1010110101010011 : ((bitCount == 0x20) ? 0b11000000110011101111111111010001 : 0b1101000001110101011001101010111011001000010011011100000101001001)),
		(bitCount == 8) ? 0b11000101 : ((bitCount == 0x10) ? 0b1001110101010101 : ((bitCount == 0x20) ? 0b10000101110111001111001010110111 : 0b1011101011011100000011111111111011100000110111011111000000001101)),
		(bitCount == 8) ? 0b10100011 : ((bitCount == 0x10) ? 0b1000111011011011 : ((bitCount == 0x20) ? 0b11000110000001011001100110100001 : 0b1011010110000101101010101100010110111111111101000011010100010111)),
	};

	// returns the multiplier which with numbers need to be multiplied with to undo a multiplication

	// a prime over (maxpoolsize / 2) and below maxpoolsize
	// it will make the number jump around

	// https://primes.utm.edu/curios/index.php?start=20&stop=24

	static constexpr t reverseP0 = math::modInverse((signedBiggerType)randomPrimes[0], maxPlus1);
	static constexpr t reverseP1 = math::modInverse((signedBiggerType)randomPrimes[1], maxPlus1);

	inline t shiftAround(t in, int bits)
	{
		return (in >> bits) | (in << (bitCount - bits));
	}
	inline t shuffle(t in)
	{
		// math::modInverse((signedBiggerType)multiplier, maxPlus1);
		in *= randomPrimes[0];
		in += randomPrimes[2]; // when more than the maximum, it will just start from 0 again
		in = shiftAround(in, 0x5);
		in *= randomPrimes[1];
		in ^= seed;
		in += randomPrimes[3];
		// in ^= shiftAround(in, 0x3); // create more impact with changing numbers
		return in;
	}
	inline t reverseShuffle(t in)
	{
		in -= randomPrimes[3];
		in ^= seed;
		in *= reverseP1;
		in = shiftAround(in, bitCount - 0x5);
		in -= randomPrimes[2];
		in *= reverseP0;
		return in;
	}
	shuffleRandom(const t &seed) : seed(seed) {}
};

typedef shuffleRandom<unsigned int> uintShuffleRandom;