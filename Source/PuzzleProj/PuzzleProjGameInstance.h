// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SPuzzleProjGameOp.h"
#include "PuzzleProjWidgetLevel.h"
#include "PuzzleProjGameInstance.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPROJ_API UPuzzleProjGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPuzzleProjGameInstance();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameOptions")
		FGameOptions GameOptions;

	UFUNCTION(BlueprintCallable, Category = "Game")
		void UpdateGameOptions(FGameOptions _GameOptions);

	UFUNCTION(BlueprintCallable, Category = "Game")
		FGameOptions GetGameOptions();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
		int32 SimpleGameScore;

	//save menu op only 
	UFUNCTION(BlueprintCallable, Category = "Save")
		void SaveGameMenuOp();
	//save score only 
	UFUNCTION(BlueprintCallable, Category = "SaveScore")
		void SaveGameScore();
	//load menu only 
	UFUNCTION(BlueprintCallable, Category = "Load")
		void LoadGameMenuOp();
	///load score only 
	UFUNCTION(BlueprintCallable, Category = "Load")
		void LoadGameScore();

	UFUNCTION(BlueprintCallable, Category = "SaveConfig")
		void SaveGameConfigs();
	///load score only 
	UFUNCTION(BlueprintCallable, Category = "LoadConfig")
		void LoadGameConfigs();

	UFUNCTION(BlueprintCallable, Category = "Load")
		void LoadAll();

	UPROPERTY()
		FGameOptions LevelOptions;

	UFUNCTION(BlueprintCallable, Category = "Game")
		void SetLevelOptions(FGameOptions _LevelOptions, UPuzzleProjWidgetLevel* _LevelWidg);

	UFUNCTION(BlueprintCallable, Category = "Game")
		void SetNextLevelOptions();

	UFUNCTION(BlueprintCallable, Category = "Level")
		FGameOptions GetLevelOptions();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
		bool isSimpleModeOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		UPuzzleProjWidgetLevel* LevelWidg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
		bool isMuteSounds;

private:
	void SetOptionsFromDif(bool levelOP = false);
};
