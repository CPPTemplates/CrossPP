#include "random.h"
std::mt19937 getGeneratorBasedOnTime()
{
	return std::mt19937{(uint)std::chrono::high_resolution_clock::now().time_since_epoch().count()};
}
std::wstring randomName(std::mt19937 &generator)
{
	static std::vector<std::wstring> randomFirstNames = {
		L"blocky",
		L"miney",
		L"crafty",
		// https://www.ssa.gov/oact/babynames/decades/century.html
		L"mary",
		L"patricia",
		L"jennifer",
		L"linda",
		L"james",
		L"robert",
		L"john",
		L"michael",
	};
	return randomFirstNames[randIndex(generator, randomFirstNames.size())];
}
