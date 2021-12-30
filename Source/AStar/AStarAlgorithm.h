#pragma once
#include "IAlgorithm.h"
#include "AStarAlgorithm.generated.h"

USTRUCT()
struct FGridData
{
	GENERATED_BODY()
	FGridData(): G(0),H(0),Coord(FIntPoint::NoneValue),ParentCoord(FIntPoint::NoneValue)
	{}
	FGridData(FIntPoint InCoord): G(0),H(0),Coord(InCoord),ParentCoord(FIntPoint::NoneValue)
	{}
	
	UPROPERTY()
	int32 G;
	UPROPERTY()
	int32 H;
	UPROPERTY()
	FIntPoint Coord;
	UPROPERTY()
	FIntPoint ParentCoord;

	FORCEINLINE virtual bool operator==(const FGridData& Other)const
	{
		return  Coord == Other.Coord;
	}
	FORCEINLINE virtual bool operator==(const FIntPoint& Point)const
	{
		return  Coord == Point;
	}
	FORCEINLINE virtual bool operator!=(const FGridData& Other)const
	{
		return !(*this == Other);
	}
	FORCEINLINE virtual bool operator!=(const FIntPoint& Point)const
	{
		return !(*this == Point);
	}
	FORCEINLINE virtual bool operator<(const FGridData& Other)const
	{
		return G + H < Other.G + Other.H;
	}
	
};


class FAStarAlgorithm final : public IAlgorithm
{
public:
	//Next-TODO: 在这设置数据结构
	//所有棋盘格的数据,只用于回溯,与本案例无直接关系
	UPROPERTY()
	TArray<FGridData> AllGrids;
	//最终的路径,与本案例无直接关系
	UPROPERTY()
	TArray<FIntPoint> Path;
    	
    	
	//开放的棋盘格
	UPROPERTY()
	TArray<FGridData> OpenGrids;
	//关闭的棋盘格
	UPROPERTY()
	TArray<FIntPoint> CloseGrids;
	

	//使用曼哈顿距离计算2点距离
	UFUNCTION()
	int32 GetDistanceManhattan(const FIntPoint& Current, const FIntPoint& Target);

	virtual FIntPoint Step(FGridMap const& Map, FIntPoint const& CurrentPoint) override;
	
	//这两个函数跟本案例无直接关系 
	//↓↓↓↓↓↓↓↓↓↓
	UFUNCTION()
	void BuildPath(const FGridData& EndPoint);
	UFUNCTION()
	void PrintDebug();
};
