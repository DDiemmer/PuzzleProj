// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleProjWidgetLevel.h"
#include "PuzzleProjSaveGame.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"


#pragma region Save e Load level
void UPuzzleProjWidgetLevel::SaveLevelScore() {
	UPuzzleProjSaveGame* SaveGameInst = Cast<UPuzzleProjSaveGame>(UGameplayStatics::CreateSaveGameObject(UPuzzleProjSaveGame::StaticClass()));
	//set variables like game instance
	SaveGameInst->LevelScore = LevelScore;
	SaveGameInst->LevelEnable = LevelEnable;
	//save
	UGameplayStatics::SaveGameToSlot(SaveGameInst, LevelName, 0);

	if (LevelScore > 0 && NextLevelWidg != nullptr)
	{
		if (!NextLevelWidg->LevelEnable) {
			NextLevelWidg->LevelEnable = true;
			NextLevelWidg->SaveLevelScore();
		}
	}
}

void UPuzzleProjWidgetLevel::LoadLevelScore()
{
	if (LevelName.Len() > 0) {
		UPuzzleProjSaveGame* LoadGameInst = Cast<UPuzzleProjSaveGame>(UGameplayStatics::CreateSaveGameObject(UPuzzleProjSaveGame::StaticClass()));
		//load save game 
		LoadGameInst = Cast<UPuzzleProjSaveGame>(UGameplayStatics::LoadGameFromSlot(LevelName, 0));
		if (LoadGameInst) {

			LevelScore = LoadGameInst->LevelScore;
			LevelEnable = LoadGameInst->LevelEnable;
		}
		else {
			LevelScore = 0.f;
		}
	}
}
#pragma endregion