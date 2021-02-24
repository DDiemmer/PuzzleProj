// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SPuzzleProjGameOp.h"
#include "PuzzleProjSaveGame.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPROJ_API UPuzzleProjSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPuzzleProjSaveGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Options")
		FGameOptions GameOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Score")
		int32 SimpleGameScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Infos")
		float LevelScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Infos")
		bool LevelEnable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
		bool isMuteSounds;

};
