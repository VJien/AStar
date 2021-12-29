#include "AStarAlgorithm.h"


int32 FAStarAlgorithm::GetDistanceManhattan(const FIntPoint& Current, const FIntPoint& Target)
{
	return FMath::Abs(Current.X - Target.X) + FMath::Abs(Current.Y - Target.Y);
}


/*
* A*寻路算法目标：
* 给定节点图，并给定起始点和终点，目标是快速找到从起始点到终点的最短路径。
* 
* 算法原理：
* 小人从起始点开始，以一步（走到相邻节点）为单位，遍历所有的节点，对每个节点计算到start点的路径距离
* 直到到达终点，或者所有点都已经被遍历（没找到路径）
* 
* 实现细节：
* close_set：小人已经走到过的节点
* open_set：小人已经发现的节点（小人已经走到过的节点的相邻节点）
* 估算路径长度公式： f(cur_node) = g(cur_node) +h(cur_node)
* g:起始点到cur_node的最短路径的确认长度
* h:cur_node到终点的路径估算长度
* f:经过cur_node的最短路径的估算长度
* 
* 1.在open_set中选择f最小的节点，遍历所有的不在close_set中的相邻点
* 2.1相邻点如果是新发现节点，那么计算g和f值，并记录路径（记录parent_node）
* 2.2如果是旧节点，那么计算g值，如果小于当前的g值，那么更新路径
* 3.相邻点遍历完成后，跳转到1
* 
* 结束条件1：目标点被找到，路径寻找成功
* 结束条件2：open_set中没有节点，路径寻找失败
 */
FIntPoint FAStarAlgorithm::Step(FGridMap const& Map, FIntPoint const& CurrentPoint)
{
	// Next-TODO: 在这实现 A* 寻路算法：
	constexpr int DirectionsCount = 4;
	const FIntPoint Directions[DirectionsCount] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
	if (OpenChess.Num() == 0)
	{
		for (int32 i = 0 ; i < DirectionsCount; ++i)
		{
			FIntPoint NewPoint = CurrentPoint + Directions[i];
			if (!CloseChess.Contains(NewPoint))
			{
				FChessData NewData(CurrentPoint);
				NewData.G = GetDistanceManhattan(CurrentPoint, NewPoint);
				NewData.H = GetDistanceManhattan(NewPoint, Map.End);
				NewData.Coord = NewPoint;
				switch (Map.GetGridStateAt(NewPoint))
				{
				case EGridState::Unreached:
					{
						OpenChess.Emplace(NewPoint,NewData);
					}
				case EGridState::Invalid:
					{
						CloseChess.AddUnique(NewPoint);
					}
				case EGridState::Wall:
					{
						CloseChess.AddUnique(NewPoint);
					}
				case EGridState::End:
					return NewPoint;
				default:
					break;
				}
			}
			
		}
	}
	if (OpenChess.Contains(Map.End))
	{
		return Map.End;
	}
	if (OpenChess.Num()>0)
	{
			OpenChess.ValueSort([](FChessData A, FChessData B)
		{
			return A < B;
		});
		return OpenChess[0].Coord;
	}
	

	
	
	
	return CurrentPoint;
};
