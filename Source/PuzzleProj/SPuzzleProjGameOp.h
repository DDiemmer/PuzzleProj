// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPuzzleProjGameOp.generated.h"

UENUM(BlueprintType)
enum EDIFFICULTY
{
	BEGINNER = 0,
	INTERMEDIATE = 1,
	EXPERT = 2,
	CUSTOM = 3,
};

USTRUCT(BlueprintType)
struct FGameOptions {

	GENERATED_USTRUCT_BODY()

public:

	FGameOptions() {
	
	}
		///Attribute Acuity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAME OPTIONS")
	TEnumAsByte<EDIFFICULTY> Difficulty;

	/** Number of blocks along each side of grid */
	UPROPERTY(Category = "GameOptions", EditAnywhere, BlueprintReadOnly)
		int32 RowSize;

	UPROPERTY(Category = "GameOptions", EditAnywhere, BlueprintReadOnly)
		int32 ColSize;
	/**Number of bombs inside blocks*/
	UPROPERTY(Category = "GameOptions", EditAnywhere, BlueprintReadOnly)
		int32 NumBombs;

	UPROPERTY(Category = "GameOptions", EditAnywhere, BlueprintReadWrite)
		int32 TimeLimit;

	UPROPERTY(Category = "GameOptions", EditAnywhere, BlueprintReadWrite)
		bool UseTimeLimit;

	UPROPERTY(Category = "GameOptions", EditAnywhere, BlueprintReadWrite)
		bool UseHelpIcon;

	UPROPERTY(Category = "GameOptions", EditAnywhere, BlueprintReadWrite)
		bool ShowNumBombs;

	UPROPERTY(Category = "GameOptions", EditAnywhere, BlueprintReadWrite)
		bool DarkMode;
};
