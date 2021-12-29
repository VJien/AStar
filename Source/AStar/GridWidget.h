// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EGridState.h"
#include "Blueprint/UserWidget.h"
#include "GridWidget.generated.h"

UCLASS()
class ASTAR_API UGridWidget final : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = AStar)
	EGridState GridState;

	UPROPERTY(BlueprintReadOnly, Category = AStar)
	FIntPoint Point;
};
