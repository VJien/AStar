#pragma once
#include "EGridState.h"

struct FGridMap final
{
	FIntPoint Start;
	FIntPoint End;
	FIntPoint MapSize;
	TMap<FIntPoint, EGridState> Grids;

	bool Init(FIntPoint const& StartPoint,
	          FIntPoint const& EndPoint,
	          FIntPoint const& Size,
	          TArray<FIntPoint> const& Walls);

	EGridState GetGridStateAt(FIntPoint const& Point) const;
};
