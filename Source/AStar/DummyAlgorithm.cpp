#include "DummyAlgorithm.h"

FIntPoint FDummyAlgorithm::Step(FGridMap const& Map, FIntPoint const& CurrentPoint)
{
	constexpr int DirectionsCount = 4;
	const FIntPoint Directions[DirectionsCount] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
	int Indices[DirectionsCount] = {0, 1, 2, 3};
	for (int i = 0; i < DirectionsCount; ++i)
	{
		int LatterIndex = FMath::RandRange(i, DirectionsCount - 1);
		Swap(Indices[i], Indices[LatterIndex]);
	}
	for (int Index : Indices)
	{
		FIntPoint const& Direction = Directions[Index];
		FIntPoint NewPoint = CurrentPoint + Direction;
		switch (Map.GetGridStateAt(NewPoint))
		{
		case EGridState::Invalid:
		case EGridState::Wall:
			continue;
		default:
			return NewPoint;
		}
	}
	return CurrentPoint;
}
