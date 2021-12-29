#pragma once
#include "GridMap.h"

class IAlgorithm
{
public:
	virtual ~IAlgorithm() = default;
	virtual FIntPoint Step(FGridMap const& Map, FIntPoint const& CurrentPoint) = 0;
};
