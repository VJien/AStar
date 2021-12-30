#include "AStarAlgorithm.h"

#include "AStarLog.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

PRAGMA_DISABLE_OPTIMIZATION

int32 FAStarAlgorithm::GetDistanceManhattan(const FIntPoint& Current, const FIntPoint& Target)
{
	return FMath::Abs(Current.X - Target.X) + FMath::Abs(Current.Y - Target.Y);
}

FIntPoint FAStarAlgorithm::Step(FGridMap const& Map, FIntPoint const& CurrentPoint)
{
	// Next-TODO: 在这实现 A* 寻路算法：
	//这里remove的是FIntPoint数据, 不是Data数据, 所以需要FGridData有相应的构造函数以及重写对应的==操作符
	OpenGrids.Remove(CurrentPoint);
	CloseGrids.AddUnique(CurrentPoint);
	if (CurrentPoint == Map.Start)
	{
		AllGrids.AddUnique(FGridData(CurrentPoint));
	}
	constexpr int DirectionsCount = 4;
	const FIntPoint Directions[DirectionsCount] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
	//上下左右4个方向的临近点查询
	for (int32 i = 0 ; i < DirectionsCount; ++i)
	{
		FIntPoint NewPoint = CurrentPoint + Directions[i];
		if (!CloseGrids.Contains(NewPoint))
		{
			//构建Data数据, parent设置为当前点用于回溯
			FGridData NewData;
			NewData.ParentCoord = CurrentPoint;
			NewData.G = GetDistanceManhattan(CurrentPoint, NewPoint);
			NewData.H = GetDistanceManhattan(NewPoint, Map.End);
			NewData.Coord = NewPoint;
			
			//如果已经在开放列表的,那么需要替换Parent
			if(OpenGrids.Contains(NewData))
			{
				int32 idx = AllGrids.Find(NewData);
				AllGrids[idx].ParentCoord = CurrentPoint;
			}
			
			
			EGridState State = Map.GetGridStateAt(NewPoint);
			if (State == EGridState::End)
			{
				//添加到All数组
				AllGrids.AddUnique(NewData);
				
				// 这个路径跟本案例无关,只是为了走一下AStar的流程
				BuildPath(NewData);
				PrintDebug();
				return NewPoint;
			}
			else if (State == EGridState::Unreached)
			{
				//添加到All数组
				AllGrids.AddUnique(NewData);
				OpenGrids.Emplace(NewData);
			}
			else
			{
				CloseGrids.AddUnique(NewPoint);
			}

		}
	}
	//排序并直接返回开销最小的点
	if (OpenGrids.Num()>0)
	{
			OpenGrids.Sort([](FGridData A, FGridData B)
		{
			return A < B;
		});
		UE_LOG(LogTemp, Warning, TEXT("路径点 = %d,%d \n"), OpenGrids[0].Coord.X, OpenGrids[0].Coord.Y);
		return OpenGrids[0].Coord;
	}
	return CurrentPoint;
};

void FAStarAlgorithm::BuildPath(const FGridData& EndPoint)
{
	Path.Empty();
	int32 currIdx = AllGrids.Find(EndPoint);
	if (!AllGrids.IsValidIndex(currIdx))
	{
		return;
	}
	FGridData CurrData = AllGrids[currIdx];
	while (CurrData.Coord!= FIntPoint::NoneValue)
	{
		Path.Emplace(CurrData.Coord);
		if (CurrData.ParentCoord != FIntPoint::NoneValue)
		{
			int32 parentIdx = AllGrids.Find(CurrData.ParentCoord);
			if (AllGrids.IsValidIndex(parentIdx))
			{
				CurrData = AllGrids[parentIdx];
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	//反转数组
	const int32 ArraySize = Path.Num();
	for (int32 i = 0, i2 = ArraySize - 1; i < ArraySize / 2 ; ++i, --i2)
	{
		Path.Swap(i, i2);
	}

}
void FAStarAlgorithm::PrintDebug()
{
	for (int32 i = 0; i< Path.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("路径 [%d] = %s \n"), i, *Path[i].ToString());
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("路径 [%d] = %s \n"),i, *Path[i].ToString()));
	}
}

PRAGMA_ENABLE_OPTIMIZATION