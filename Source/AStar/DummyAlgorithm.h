#pragma once
#include "IAlgorithm.h"

class FDummyAlgorithm final : public IAlgorithm
{
public:
	virtual FIntPoint Step(FGridMap const& Map, FIntPoint const& CurrentPoint) override;
};
