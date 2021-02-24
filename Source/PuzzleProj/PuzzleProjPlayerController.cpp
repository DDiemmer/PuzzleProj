// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PuzzleProjPlayerController.h"
#include "Engine/GameEngine.h"

APuzzleProjPlayerController::APuzzleProjPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;

	DefaultMouseCursor = EMouseCursor::Crosshairs;
	//	GEngine->AddOnScreenDebugMessage(-1, 35.0f, FColor::Red, FString::Printf(TEXT("Constructplayer")));
}

void APuzzleProjPlayerController::UpBlockToUnlock(int32 numBlock)
{
	NumBlocksUnlock -= numBlock;
	//verificar se ganhou 
	if (NumBlocksUnlock <= 0)
	{
		IsWinner = true;
		IsGameOver = true;
	}
}

void APuzzleProjPlayerController::SetAddNumBomb(bool Add)
{
	if (Add)
		NumBombsUnlock++;
	else
		NumBombsUnlock--;
}

void APuzzleProjPlayerController::SetGameOver(bool isOver)
{
	IsGameOver = isOver;
}

void APuzzleProjPlayerController::SetPlayerCursor(EPlayerCursor CursorType)
{
	PlayerCursor = CursorType;
}

int32 APuzzleProjPlayerController::GetNumBombsUnlock()
{
	return NumBombsUnlock;
}

bool APuzzleProjPlayerController::GetIsGameOver()
{
	return IsGameOver;
}

void APuzzleProjPlayerController::UpdateGameOptions(FGameOptions _GameOptions)
{
	GameOptions = _GameOptions;
	//set num blocks to unlock 
	NumBlocksUnlock = (GameOptions.ColSize * GameOptions.RowSize) - GameOptions.NumBombs;
}

void APuzzleProjPlayerController::SetLastPlayerCursor(EPlayerCursor CursorType)
{
	LastRightMouseCLickCursor = CursorType;
}

void APuzzleProjPlayerController::SetIgnoreTouch(bool Ignore)
{
	TouchIgnore = Ignore;
}

bool APuzzleProjPlayerController::GetIgnoreTouch()
{
	return TouchIgnore;
}



