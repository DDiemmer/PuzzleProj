// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "PuzzleProjEnumPC.generated.h"

UENUM(BlueprintType)
enum EPlayerCursor {
	OPEN = 0,
	FLAG = 1,
	QUEST = 2,
};

UENUM(BlueprintType)
enum EFSIDE
{
	NONES = 0,
	BACKS = 1,
	FRONTS = 2,
	TOPS = 3,
	DOWNS = 4,
};