#include "globalFunctions.h"
#include "math/timemath.h"
#pragma once
template<typename t>
struct benchMarkPoint
{
	benchMarkPoint(const t& processingType):processingType(processingType),measureTime(getmicroseconds()){}
	t processingType;
	microseconds measureTime;
};

template<typename t>
struct benchmarkContainer
{
	std::vector<benchMarkPoint<t>> benchMarks;
	inline microseconds measureBenchmarkTime(const t& processingType) const
	{
		microseconds totalMicroSeconds = 0;
		for (size_t i = 0; i < benchMarks.size(); i++)
		{
			const benchMarkPoint<t> point = benchMarks[i];
			if (point.processingType == processingType && i + 1 < benchMarks.size())
			{
				totalMicroSeconds += benchMarks[i + 1].measureTime - point.measureTime;
			}
		}
		return totalMicroSeconds;
	}
	inline microseconds measureTotalBenchmarkTime() const
	{
		return benchMarks.size() > 1 ? benchMarks[benchMarks.size() - 1].measureTime - benchMarks[0].measureTime : 0;
	}
	inline void removeOldBenchmarks()
	{
		benchMarks = std::vector<benchMarkPoint<t>>({ benchMarks[benchMarks.size() - 1] });
	}
	inline void addBenchmarkPoint(const t& processingType) {
		benchMarks.push_back(benchMarkPoint<t>(processingType));
	}
};