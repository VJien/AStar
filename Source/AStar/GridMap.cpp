#include "GridMap.h"
#include "AStarLog.h"

static bool CheckMapSizeValid(FIntPoint const& Size)
{
	int64 GridsCount = static_cast<int64>(Size.X) * static_cast<int64>(Size.Y);
	bool bIsSizePositive = Size.X >= 0 && Size.Y >= 0;
	bool bIsSizeOverflow = GridsCount > TNumericLimits<int32>::Max();
	return bIsSizePositive && ! bIsSizeOverflow;
}

static bool CheckPointValid(FIntPoint const& Point, FIntPoint const& Size)
{
	bool bIsPointInMapX = Point.X >= 0 && Point.X < Size.X;
	bool bIsPointInMapY = Point.Y >= 0 && Point.Y < Size.Y;
	return bIsPointInMapX && bIsPointInMapY;
}

bool FGridMap::Init(
	FIntPoint const& StartPoint,
	FIntPoint const& EndPoint,
	FIntPoint const& Size,
	TArray<FIntPoint> const& Walls)
{
	//0. 配置检查
	if (!CheckMapSizeValid(Size))
	{
		UE_LOG(LogAStar, Error, TEXT("%s 地图大小 (%d, %d) 不合法"), TEXT(__FUNCTION__), Size.X, Size.Y);
		return false;
	}
	if (!CheckPointValid(StartPoint, Size))
	{
		UE_LOG(LogAStar, Error, TEXT("%s 起点 (%d, %d) 不合法"), TEXT(__FUNCTION__), StartPoint.X, StartPoint.Y);
		return false;
	}
	if (!CheckPointValid(EndPoint, Size))
	{
		UE_LOG(LogAStar, Error, TEXT("%s 终点 (%d, %d) 不合法"), TEXT(__FUNCTION__), EndPoint.X, EndPoint.Y);
		return false;
	}
	for (FIntPoint const& Wall : Walls)
	{
		if (!CheckPointValid(Wall, Size))
		{
			UE_LOG(LogAStar, Error, TEXT("%s 墙体 (%d, %d) 不合法"), TEXT(__FUNCTION__), Wall.X, Wall.Y);
			return false;
		}
		if (Wall == Start || Wall == End)
		{
			UE_LOG(LogAStar, Error, TEXT("%s 墙体 (%d, %d) 与起点或终点重合"), TEXT(__FUNCTION__), Wall.X, Wall.Y);
			return false;			
		}
	}

	//1. 初始化基础变量
	Start = StartPoint;
	End = EndPoint;
	MapSize = Size;
	
	//2. 添加格子状态
	for (int32 X = 0; X < Size.X; ++X)
	{
		for (int32 Y = 0; Y < Size.Y; ++Y)
		{
			FIntPoint Point = {X, Y};
			Grids.Add(Point, EGridState::Unreached);
		}
	}
	
	//3. 设置特殊格子状态
	Grids.Add(Start, EGridState::Start);
	Grids.Add(End, EGridState::End);
	for (FIntPoint const& Wall : Walls)
		Grids.Add(Wall, EGridState::Wall);
		
	return true;
}

EGridState FGridMap::GetGridStateAt(FIntPoint const& Point) const
{
	if (const EGridState* GridStateFound = Grids.Find(Point))
	{
		return *GridStateFound;
	}
	return EGridState::Invalid;
}
