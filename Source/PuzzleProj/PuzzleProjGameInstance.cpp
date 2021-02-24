// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleProjGameInstance.h"
#include "PuzzleProjSaveGame.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"



UPuzzleProjGameInstance::UPuzzleProjGameInstance() {

	UPuzzleProjGameInstance::LoadAll();
}

void UPuzzleProjGameInstance::UpdateGameOptions(FGameOptions _GameOptions)
{
	GameOptions = _GameOptions;
	UPuzzleProjGameInstance::SetOptionsFromDif();
}

FGameOptions UPuzzleProjGameInstance::GetGameOptions()
{
	return GameOptions;
}

FGameOptions UPuzzleProjGameInstance::GetLevelOptions()
{
	return LevelOptions;
}

void UPuzzleProjGameInstance::SetLevelOptions(FGameOptions _LevelOptions, UPuzzleProjWidgetLevel* _LevelWidg)
{
	LevelWidg = _LevelWidg;
	LevelOptions = _LevelOptions;
	isSimpleModeOn = false;
	UPuzzleProjGameInstance::SetOptionsFromDif(true);
}

void UPuzzleProjGameInstance::SetNextLevelOptions()
{
	if (LevelWidg != nullptr) {
		if (LevelWidg->NextLevelWidg != nullptr)
		{
			LevelOptions = LevelWidg->NextLevelWidg->GameOptions;
			isSimpleModeOn = false;
			LevelWidg = LevelWidg->NextLevelWidg;
			UPuzzleProjGameInstance::SetOptionsFromDif(true);
		}
	}
}

#pragma region Save Load Game MenuOP Only
//save menu
void UPuzzleProjGameInstance::SaveGameMenuOp()
{
	UPuzzleProjSaveGame* SaveGameInst = Cast<UPuzzleProjSaveGame>(UGameplayStatics::CreateSaveGameObject(UPuzzleProjSaveGame::StaticClass()));
	//set variables like game instance
	SaveGameInst->GameOptions = GameOptions;
	//save
	UGameplayStatics::SaveGameToSlot(SaveGameInst, TEXT("CMSlotMenuOp"), 0);
}

void UPuzzleProjGameInstance::LoadGameMenuOp()
{
	UPuzzleProjSaveGame* SaveGameInst = Cast<UPuzzleProjSaveGame>(UGameplayStatics::CreateSaveGameObject(UPuzzleProjSaveGame::StaticClass()));
	//load save game 
	SaveGameInst = Cast<UPuzzleProjSaveGame>(UGameplayStatics::LoadGameFromSlot("CMSlotMenuOp", 0));
	if (SaveGameInst) {
		GameOptions = SaveGameInst->GameOptions;
		UPuzzleProjGameInstance::SetOptionsFromDif();

	}
	else {
		GameOptions.Difficulty = EDIFFICULTY::BEGINNER;
		UPuzzleProjGameInstance::SetOptionsFromDif();

		GameOptions.TimeLimit = 0;
		GameOptions.UseHelpIcon = true;
		GameOptions.ShowNumBombs = true;
		GameOptions.UseTimeLimit = false;
		GameOptions.DarkMode = false;
	}
}
#pragma endregion

#pragma region Save Load Score only
void UPuzzleProjGameInstance::SaveGameScore()
{
	UPuzzleProjSaveGame* SaveGameInst = Cast<UPuzzleProjSaveGame>(UGameplayStatics::CreateSaveGameObject(UPuzzleProjSaveGame::StaticClass()));
	SaveGameInst->SimpleGameScore = SimpleGameScore;
	//save
	UGameplayStatics::SaveGameToSlot(SaveGameInst, TEXT("CMSlotScore"), 0);
}


void UPuzzleProjGameInstance::LoadGameScore()
{
	UPuzzleProjSaveGame* SaveGameInst = Cast<UPuzzleProjSaveGame>(UGameplayStatics::CreateSaveGameObject(UPuzzleProjSaveGame::StaticClass()));
	//load save game 
	SaveGameInst = Cast<UPuzzleProjSaveGame>(UGameplayStatics::LoadGameFromSlot("CMSlotScore", 0));
	if (SaveGameInst) {
		SimpleGameScore = SaveGameInst->SimpleGameScore;
		UPuzzleProjGameInstance::SetOptionsFromDif();
	}
	else {
		SimpleGameScore = 0;
	}
}
#pragma endregion

#pragma region Save Load Game Config
void UPuzzleProjGameInstance::SaveGameConfigs()
{
	UPuzzleProjSaveGame* SaveGameInst = Cast<UPuzzleProjSaveGame>(UGameplayStatics::CreateSaveGameObject(UPuzzleProjSaveGame::StaticClass()));
	SaveGameInst->isMuteSounds = isMuteSounds;
	//save
	UGameplayStatics::SaveGameToSlot(SaveGameInst, TEXT("CMSlotConfigs"), 0);
}
void UPuzzleProjGameInstance::LoadGameConfigs()
{
	UPuzzleProjSaveGame* LoadGameInst = Cast<UPuzzleProjSaveGame>(UGameplayStatics::CreateSaveGameObject(UPuzzleProjSaveGame::StaticClass()));
	//load save game 
	LoadGameInst = Cast<UPuzzleProjSaveGame>(UGameplayStatics::LoadGameFromSlot("CMSlotConfigs", 0));
	if (LoadGameInst) {
		isMuteSounds = LoadGameInst->isMuteSounds;
		UPuzzleProjGameInstance::SetOptionsFromDif();
	}
	else {
		isMuteSounds = false;
	}
}
#pragma endregion

void UPuzzleProjGameInstance::LoadAll()
{
	UPuzzleProjGameInstance::LoadGameMenuOp();
	UPuzzleProjGameInstance::LoadGameScore();
	UPuzzleProjGameInstance::LoadGameConfigs();
}

void UPuzzleProjGameInstance::SetOptionsFromDif(bool levelOP)
{
	if (levelOP) {
		switch (LevelOptions.Difficulty)
		{
		case EDIFFICULTY::BEGINNER:

			LevelOptions.RowSize = 8;
			LevelOptions.ColSize = 8;
			LevelOptions.NumBombs = 10;
			break;

		case EDIFFICULTY::INTERMEDIATE:
			LevelOptions.RowSize = 16;
			LevelOptions.ColSize = 16;
			LevelOptions.NumBombs = 40;
			break;
		case EDIFFICULTY::EXPERT:
			LevelOptions.RowSize = 31;
			LevelOptions.ColSize = 16;
			LevelOptions.NumBombs = 99;
			break;
		default:
			break;
		}
	}
	else {
		switch (GameOptions.Difficulty)
		{
		case EDIFFICULTY::BEGINNER:

			GameOptions.RowSize = 8;
			GameOptions.ColSize = 8;
			GameOptions.NumBombs = 10;
			break;

		case EDIFFICULTY::INTERMEDIATE:
			GameOptions.RowSize = 16;
			GameOptions.ColSize = 16;
			GameOptions.NumBombs = 40;
			break;
		case EDIFFICULTY::EXPERT:
			GameOptions.RowSize = 31;
			GameOptions.ColSize = 16;
			GameOptions.NumBombs = 99;
			break;
		default:
			break;
		}
	}
}
