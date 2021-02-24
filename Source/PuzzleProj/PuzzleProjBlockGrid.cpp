// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PuzzleProjBlockGrid.h"
#include "PuzzleProjPlayerController.h"
#include "PuzzleProjBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Engine/StaticMesh.h"

//#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

APuzzleProjBlockGrid::APuzzleProjBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	/*ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f, 0.f, 0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->SetupAttachment(DummyRoot);*/
	//GEngine->AddOnScreenDebugMessage(-1, 35.0f, FColor::Red, FString::Printf(TEXT("Construct Grid ")));

}
void APuzzleProjBlockGrid::GetPlayerOptions(APuzzleProjPlayerController * PlayerController)
{
	
	if (PlayerController) {
		//GEngine->AddOnScreenDebugMessage(-1, 35.0f, FColor::Black, FString::Printf(TEXT("Player found %d"), PlayerController->GameOptions.ColSize));
		RowSize = PlayerController->GameOptions.RowSize;
		ColSize = PlayerController->GameOptions.ColSize;
		NumBombs = PlayerController->GameOptions.NumBombs;
	}
	if (RowSize == 0 || ColSize == 0)
	{
		// Set defaults
		RowSize = 8;
		ColSize = 8;
		NumBombs = 10;
	}

	if (NumBombs > RowSize * ColSize) {
		NumBombs = (ColSize * RowSize) - 1;
	}
}


void APuzzleProjBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	APuzzleProjPlayerController *PlayerController = Cast<APuzzleProjPlayerController>(GetWorld()->GetFirstPlayerController());

	APuzzleProjBlockGrid::GetPlayerOptions(PlayerController);

	//if (GetWorld())
	//{
	//	UGameViewportClient* Viewport = GetWorld()->GetGameViewport();
	//	FIntPoint ViewSize = Viewport->Viewport->GetSizeXY();
	//	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("Size X = %i , Size Y = %i"), ViewSize.X, ViewSize.Y));
	//}

	const float multiplayFactorSpace = 265.f;

	float sizeFactor = 8.f / ColSize; //1 para 8  0,5 p 16  
	BlockSpacing = multiplayFactorSpace * sizeFactor; // 260 * 1 

	FVector fSizeBlock = FVector(sizeFactor, sizeFactor, sizeFactor);

	APuzzleProjBlockGrid::SetBombsPos();

	//GEngine->AddOnScreenDebugMessage(-1, 35.0f, FColor::Red, FString::Printf(TEXT("Begin play Grid ")));

	// Number of blocks
	const int32 NumBlocks = RowSize * ColSize;

	int32 counterBlocks = 0;
	int32 itBombsPos = 0;
	float xAjust = (BlockSpacing * (ColSize / 2)) - (BlockSpacing / 2);
	float yAjust = (BlockSpacing * (RowSize / 2)) - (BlockSpacing / 2);

	// Loop to spawn each block
	TArray<APuzzleProjBlock*> Blocks;

	for (int32 BlockColIndex = 0; BlockColIndex < ColSize; BlockColIndex++)
	{
		const float XOffset = (-(BlockColIndex)* BlockSpacing) + xAjust; // Divide by dimension

		for (int32 BlockRowIndex = 0; BlockRowIndex < RowSize; BlockRowIndex++)
		{
			counterBlocks++;
			const float YOffset = ((BlockRowIndex)* BlockSpacing) - yAjust; // Modulo gives remainder

				// Make position vector, offset from Grid location
			const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

			// Spawn a block
			APuzzleProjBlock* NewBlock = GetWorld()->SpawnActor<APuzzleProjBlock>(BlockLocation, FRotator(0, 0, 0));

			// Tell the block about its owner
			if (NewBlock != nullptr)
			{
				if (PlayerController)
					NewBlock->SetPlayerController(PlayerController);

				NewBlock->SetScaleBlock(fSizeBlock, sizeFactor);

				NewBlock->OwningGrid = this;

				//set bombs on blocks 
				if (itBombsPos < NumBombs) {
					if (bombpos[itBombsPos] == counterBlocks) {
						NewBlock->SetBomb();
						NewBlock->SetBlockNumber(counterBlocks,false);
						itBombsPos++;
					}
				}
				//set number position on block
				//NewBlock->SetBlockNumber(counterBlocks);

				//Row e col estao invertidos na tela por x e y 
					///set back and inside back set front in back 
				if (BlockRowIndex > 0) {

					if (Blocks[counterBlocks - 2])
						NewBlock->SetBackBlock(Blocks[counterBlocks - 2]);
				}
				//set top and inside set down 

				if (BlockColIndex > 0) {

					int32 indexTop = (counterBlocks - RowSize) - 1;
					/*GEngine->AddOnScreenDebugMessage(-1, 35.0f, FColor::Red, FString::Printf(TEXT("Num %d"), counterBlocks));
					GEngine->AddOnScreenDebugMessage(-1, 35.0f, FColor::Blue, FString::Printf(TEXT("Num %d"),indexTop));*/

					if (Blocks[indexTop]) {
						NewBlock->SetTopBlock(Blocks[indexTop]);
					}
				}
				Blocks.Add(NewBlock);
			}
		}
	}
}


void APuzzleProjBlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	//ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

int32 APuzzleProjBlockGrid::GetNextRandBombPos(int32 maxRange)
{
	int32 val = (FMath::RandRange(1, maxRange));

	if (bombpos.Find(val) >= 0)
		return APuzzleProjBlockGrid::GetNextRandBombPos(maxRange);

	return val;
}

void APuzzleProjBlockGrid::SetBombsPos()
{

	bombpos.Init(0, NumBombs);

	for (int32 i = 0; i < NumBombs; i++)
	{
		bombpos[i] = APuzzleProjBlockGrid::GetNextRandBombPos(ColSize * RowSize);
	}

	bombpos.Sort();
}

//#undef LOCTEXT_NAMESPACE
