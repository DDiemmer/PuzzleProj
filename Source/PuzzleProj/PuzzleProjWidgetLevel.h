// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPuzzleProjGameOp.h"
#include "PuzzleProjWidgetLevel.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class PUZZLEPROJ_API UPuzzleProjWidgetLevel : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "SaveScore")
		void SaveLevelScore();

	UFUNCTION(BlueprintCallable, Category = "LoadScore")
		void LoadLevelScore();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infos")
		float LevelScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infos")
		bool LevelEnable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infos")
		FString LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infos")
		int32 LevelNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NextLevel")
		UPuzzleProjWidgetLevel* NextLevelWidg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Options")
		FGameOptions GameOptions;
};

