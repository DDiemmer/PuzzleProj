// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PuzzleProjEnumPC.h"
#include "SPuzzleProjGameOp.h"
#include "PuzzleProjPlayerController.generated.h"

/** PlayerController class used to enable cursor */

UCLASS()
class APuzzleProjPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APuzzleProjPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Cursor")
		TEnumAsByte<EPlayerCursor> PlayerCursor = EPlayerCursor::OPEN;

	UFUNCTION()
		void UpBlockToUnlock(int32 numBlock = 1);

	UFUNCTION()
		void SetAddNumBomb(bool Add = true);
	UFUNCTION(BlueprintCallable, Category = "Game")
		void SetGameOver(bool isOver = true);

	UFUNCTION(BlueprintCallable, Category = "Player Cursor")
		void SetPlayerCursor(EPlayerCursor CursorType);

	/**Accessor function for numBombosunlock */
	UFUNCTION(BlueprintPure, Category = "UI Playing")
		int32 GetNumBombsUnlock();
	/**Accessor function for isGameOver */
	UFUNCTION(BlueprintPure, Category = "UI Playing")
		bool GetIsGameOver();

	UFUNCTION(BlueprintCallable, Category = "UI Playing")
		void UpdateGameOptions(FGameOptions _GameOptions);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameOptions")
		FGameOptions GameOptions;

	///only pc 
	UPROPERTY()
		TEnumAsByte<EPlayerCursor> LastRightMouseCLickCursor = EPlayerCursor::QUEST;

	UFUNCTION()
		void SetLastPlayerCursor(EPlayerCursor CursorType);

	UFUNCTION(BlueprintCallable, Category = "Touch Events")
		void SetIgnoreTouch(bool Ignore);

	UFUNCTION(BlueprintCallable, Category = "Touch Events")
		bool GetIgnoreTouch();

protected:
	//to cpp to blueprint call 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Bombs)
		int32 NumBombsUnlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Bombs)
		bool IsGameOver;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Bombs)
		bool IsWinner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Bombs)
		int32 NumBlocksUnlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mode)
		bool isSimpleMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TouchEvents)
		bool TouchIgnore;


};


