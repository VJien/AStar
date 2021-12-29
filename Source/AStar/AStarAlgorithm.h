#pragma once
#include "IAlgorithm.h"
#include "AStarAlgorithm.generated.h"

USTRUCT()
struct FChessData
{
	GENERATED_BODY()
	FChessData(): G(0),H(0),Coord(0,0),ParentCoord(0,0)
	{}
	FChessData(FIntPoint Parent):G(0),H(0),Coord(0,0),ParentCoord(Parent)
	{}
	UPROPERTY()
	int32 G;
	UPROPERTY()
	int32 H;
	UPROPERTY()
	FIntPoint Coord;
	UPROPERTY()
	FIntPoint ParentCoord;

	FORCEINLINE virtual bool operator==(const FChessData& Other)const
	{
		return G == Other.G && H == Other.H && Coord == Other.Coord && ParentCoord == Other.ParentCoord;
	}
	FORCEINLINE virtual bool operator!=(const FChessData& Other)const
	{
		return !(*this == Other);
	}
	FORCEINLINE virtual bool operator<(const FChessData& Other)const
	{
		return G + H < Other.G + Other.H;
	}
	
};


class FAStarAlgorithm final : public IAlgorithm
{
public:
	//Next-TODO: 在这设置数据结构
	UPROPERTY()
	TMap<FIntPoint, FChessData> OpenChess;
	UPROPERTY()
	TArray<FIntPoint> CloseChess;

	
	UFUNCTION()
	int32 GetDistanceManhattan(const FIntPoint& Current, const FIntPoint& Target);



	
	virtual FIntPoint Step(FGridMap const& Map, FIntPoint const& CurrentPoint) override;
};
