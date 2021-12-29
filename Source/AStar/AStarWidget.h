// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridMap.h"
#include "GridWidget.h"
#include "IAlgorithm.h"
#include "Blueprint/UserWidget.h"
#include "AStarWidget.generated.h"

UCLASS()
class ASTAR_API UAStarWidget final : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = AStar)
	TSubclassOf<UGridWidget> GridWidgetClass;

	UPROPERTY()
	class UCanvasPanel* GridCanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AStar)
	FIntPoint Start;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AStar)
	FIntPoint End;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AStar)
	FIntPoint MapSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AStar)
	TArray<FIntPoint> Walls;

	UPROPERTY(BlueprintReadOnly, Category = AStar)
	FIntPoint Current;

	UPROPERTY()
	TMap<FIntPoint, UGridWidget*> GridWidgets;

	FGridMap GridMap;

	TUniquePtr<IAlgorithm> Algorithm;

	UFUNCTION(BlueprintCallable, Category = AStar)
	void Calculate();

	UFUNCTION(BlueprintCallable, Category = AStar)
	void Init();
};
