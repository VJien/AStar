#pragma once

UENUM(BlueprintType)
enum class EGridState : uint8
{
	Invalid,
	Current, // 当前走到的点
	Unreached, // 未走到的点
	Reached, // 走到过的点
	Start, // 起点
	End, // 终点
	Wall, // 墙体
	Final
};
