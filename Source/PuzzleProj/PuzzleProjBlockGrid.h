// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleProjBlock.h"
#include "PuzzleProjBlockGrid.generated.h"

/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class APuzzleProjBlockGrid : public AActor
{
	GENERATED_BODY()

		/** Dummy root component */
		UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* DummyRoot;

	/** Text component for the score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTextRenderComponent* ScoreText;

public:
	APuzzleProjBlockGrid();

	/** How many blocks have been clicked */
	int32 Score;

	/** Number of blocks along each side of grid */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		int32 RowSize;
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		int32 ColSize;
	/**Number of bombs inside blocks*/
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		int32 NumBombs;

	/** Spacing of blocks */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		float BlockSpacing;

protected:
	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

public:

	/** Handle the block being clicked */
	void AddScore();

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns ScoreText subobject **/
	FORCEINLINE class UTextRenderComponent* GetScoreText() const { return ScoreText; }

private:
	TArray<int32> bombpos;

	int32 GetNextRandBombPos(int32 maxRange);

	void SetBombsPos();

	void GetPlayerOptions(APuzzleProjPlayerController* PlayerController);

};



