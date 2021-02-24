// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PuzzleProjGameMode.h"
#include "PuzzleProjPlayerController.h"
#include "PuzzleProjPawn.h"

APuzzleProjGameMode::APuzzleProjGameMode()
{
	// no pawn by default
	DefaultPawnClass = APuzzleProjPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = APuzzleProjPlayerController::StaticClass();
}
