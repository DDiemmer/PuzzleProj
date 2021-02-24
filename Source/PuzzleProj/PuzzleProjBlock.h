// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleProjEnumPC.h"
#include "PuzzleProjBlock.generated.h"

/** A block that can be clicked */
UCLASS(minimalapi)
class APuzzleProjBlock : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BlockMesh;

	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTextRenderComponent* NText;

public:
	APuzzleProjBlock();

	/** Are we currently active? */
	bool bIsActive;

	/** Pointer to white material used on the focused block */
	UPROPERTY()
		class UMaterialInstance* BaseMaterial;

	/** Pointer to blue material used on inactive blocks */
	UPROPERTY()
		class UMaterialInstance* BlueMaterial;

	UPROPERTY()
		class UMaterialInstance* OnFlagMaterial;

	UPROPERTY()
		class UMaterialInstance* OnFlagMaterialLightOn;

	UPROPERTY()
		class UMaterialInstance* OnQuestionMaterial;

	UPROPERTY()
		class UMaterialInstance* OnQuestionMaterialLightOn;

	UPROPERTY()
		class UMaterialInstance* OnOpenMaterial;

	UPROPERTY()
		class UMaterialInstance* OnOpenMaterialLightOn;

	UPROPERTY()
		class UMaterialInstance* OrangeMaterial;

	/** Pointer to red blink material used on active bomb blocks */
	UPROPERTY()
		class UMaterialInstance* RedBlinkMaterial;
	/** Grid that owns us */
	UPROPERTY()
		class APuzzleProjBlockGrid* OwningGrid;

	/** Handle the block being clicked */
	UFUNCTION()
		void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	/** Handle the block being touched  */
	UFUNCTION()
		void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
		void OnFingerPressedEndBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
		void OnFingerPressedLeaveBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	void HandleClicked();

	void Highlight(bool bOn, bool DarkModeOn = false);

	void HighlightDarkParents(bool bOn);

	void SetBomb();

	void SetBlockNumber(int32 _blockNumber, bool setText = true);

	bool GetIsBomb();

	int32 GetBlockNumber();

	bool alreadyVerifyNumBombs;
	int32 NumBombsAround;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	void SetBackBlock(APuzzleProjBlock* _BackBlock);
	void SetTopBlock(APuzzleProjBlock* _TopBlock);

	bool HasFlag();
	bool HasQuestion();


protected:



private:
	void SetFrontBlock(APuzzleProjBlock* _FrontBlock);

	void SetDownBlock(APuzzleProjBlock* _DownBlock);

	void CenterViewportCursor();

	UFUNCTION()
		int32 GetBombsAround(bool VerifyBackFront, bool VerifytopDown, bool SetNumberInZeroBombs = false, EFSIDE SIDE = EFSIDE::NONES);

	bool isBomb;

	bool Alreadyused;

	int32 blockNumber;

	float FatorMult;

	float ImpulseRadius;

	bool hasFlag;

	bool hasQuestion;

	int32 DarkMode;

	UFUNCTION()
		void Explode();
	UFUNCTION()
		void ActiveTimerExplosion(float TimeDelay);

	UPROPERTY()
		class APuzzleProjBlock* BackBlock;
	UPROPERTY()
		class APuzzleProjBlock* FrontBlock;
	UPROPERTY()
		class APuzzleProjBlock* TopBlock;
	UPROPERTY()
		class APuzzleProjBlock* DownBlock;

	UPROPERTY()
		class APuzzleProjFlag* Flag;

	UPROPERTY()
		class APuzzleProjQuestion* QuestionSign;

	void SetphysicsAtt();
	void PropagatePhysicsAtt(EFSIDE SIDE, bool onlyOneDirection = false);
	bool tempReal = false;

	UPROPERTY()
		class APuzzleProjPlayerController* PlayerController;

	bool TouchBeginHere;

public:
	UFUNCTION()
		void SetScaleBlock(FVector ScaleBlock, float factorMult);
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }
	UFUNCTION()
		void SetPlayerController(APuzzleProjPlayerController* _PlayerController);
};



