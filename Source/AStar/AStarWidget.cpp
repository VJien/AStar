// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarWidget.h"

#include "AStar.h"
#include "AStarAlgorithm.h"
#include "AStarLog.h"
#include "DummyAlgorithm.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

namespace
{
	constexpr float GridSize = 50;
	constexpr float GridMargin = 10;

	FVector2D GetGridSize()
	{
		return {GridSize, GridSize};
	}

	FVector2D GetGridPosition(const FIntPoint& Point)
	{
		return FVector2D(Point) * (GridSize + GridMargin);
	}
}

void UAStarWidget::Init()
{
	//0. 初始化grid
	if (!GridMap.Init(Start, End, MapSize, Walls))
	{
		UE_LOG(LogAStar, Error, TEXT("%s 初始化失败"), TEXT(__FUNCTION__));
		return;
	}

	//1. 初始化canvas
	GridCanvasPanel = Cast<UCanvasPanel>(WidgetTree->FindWidget(TEXT("CanvasPanel_Grids")));
	if (!GridCanvasPanel)
	{
		UE_LOG(LogAStar, Error, TEXT("%s 找不到 GridCanvasPanel"), TEXT(__FUNCTION__));
		return;
	}
	if (!GridWidgetClass)
	{
		UE_LOG(LogAStar, Error, TEXT("%s 没有设置 GridWidgetClass"), TEXT(__FUNCTION__));
		return;
	}
	
	//2. 删去旧的地图格
	for (auto const& Pair : GridWidgets)
	{
		UGridWidget* GridWidget = Pair.Value;
		GridWidget->GridState = EGridState::Invalid;
		GridWidget->RemoveFromParent();
	}
	
	//3. 添加新格
	int32 NewGridCount = MapSize.X * MapSize.Y;
	GridWidgets.Empty(NewGridCount);
	for (int32 X = 0; X < MapSize.X; ++X)
	{
		for (int32 Y = 0; Y < MapSize.Y; ++Y)
		{
			FIntPoint Point = {X, Y};
			if (UGridWidget* GridWidget = NewObject<UGridWidget>(GridCanvasPanel, GridWidgetClass))
			{
				if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(GridCanvasPanel->AddChild(GridWidget)))
				{
					CanvasSlot->SetSize(GetGridSize());
					CanvasSlot->SetPosition(GetGridPosition(Point));
				}
				GridWidget->Point = Point;
				GridWidget->GridState = EGridState::Unreached;
				GridWidgets.Add(Point, GridWidget);
			}
		}
	}
	
	//4. 设置新格状态,当前点
	GridWidgets[Start]->GridState = EGridState::Start;
	GridWidgets[End]->GridState = EGridState::End;
	for (FIntPoint const& Wall : Walls)
	{
		GridWidgets[Wall]->GridState = EGridState::Wall;
	}
	Current = Start;
	
	//5. 初始化算法
	// Next-TODO: 实现并换成 A* 算法
	 Algorithm = MakeUnique<FAStarAlgorithm>();
	//Algorithm = MakeUnique<FDummyAlgorithm>();
}

void UAStarWidget::Calculate()
{
	//0. 防御
	if (!Algorithm)
	{
		UE_LOG(LogAStar, Error, TEXT("%s 没有设置算法"), TEXT(__FUNCTION__));
		return;
	}

	//1. 如果不是终点, 则继续
	if (Current != End)
	{
		//1.1 调用算法获取下一个格子
		FIntPoint Result = Algorithm->Step(GridMap, Current);
		UGridWidget** ResultGridWidgetFound = GridWidgets.Find(Result);
		if (!ResultGridWidgetFound)
		{
			UE_LOG(LogAStar, Error, TEXT("%s 目标点 (%d, %d) 错误，当前点为 (%d, %d)"),TEXT(__FUNCTION__), Result.X, Result.Y, Current.X, Current.Y);
			return;
		}
		
		UE_LOG(LogAStar, Log, TEXT("%s 移动 (%d, %d) -> (%d, %d)"), TEXT(__FUNCTION__), Current.X, Current.Y, Result.X, Result.Y);

		//1.2 将之前的格子状态设置成走过
		GridWidgets[Current]->GridState = EGridState::Reached;

		//1.3 切换到当前找到点, 设置成当前
		Current = Result;
		UGridWidget* Widget = *ResultGridWidgetFound;
		Widget->GridState = EGridState::Current;
	}
}
