// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PuzzleProjPawn.h"
#include "PuzzleProjBlock.h"
#include "PuzzleProjEnumPC.h"
#include "PuzzleProjPlayerController.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Engine/GameEngine.h"
#include "PuzzleProjGameInstance.h"


APuzzleProjPawn::APuzzleProjPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}


void APuzzleProjPawn::BeginPlay()
{
	Super::BeginPlay();

	UPuzzleProjGameInstance* GameInstance = Cast<UPuzzleProjGameInstance>(GetGameInstance());
	if (GameInstance) {
		if (GameInstance->isSimpleModeOn)
			DarkModeOn = GameInstance->GameOptions.DarkMode;
		else
			DarkModeOn = GameInstance->LevelOptions.DarkMode;
	}

}

void APuzzleProjPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APuzzleProjPlayerController* PlayerController = Cast<APuzzleProjPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PlayerController)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UCameraComponent * OurCamera = PlayerController->GetViewTarget()->FindComponentByClass<UCameraComponent>())
			{
				FVector Start = OurCamera->GetComponentLocation();
				FVector End = Start + (OurCamera->GetComponentRotation().Vector() * 4000.0f);

				TraceForBlock(&Start, &End, true, PlayerController);
			}
		}
		else
		{
			FVector Start, Dir, End;

			float locX, LocY;

			if (PlayerController->GetMousePosition(locX, LocY)) {
				PlayerController->DeprojectScreenPositionToWorld(locX, LocY, Start, Dir);
				End = Start + (Dir * 4000.0f);
				TraceForBlock(&Start, &End, false, PlayerController);
			}

			//if (PlayerController->DeprojectMousePositionToWorld(Start, Dir))
			//{
			//	End = Start + (Dir * 4000.0f);
			//	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Blue, FString::Printf(TEXT("start Y: %f "),Start.Y));
			//	TraceForBlock(&Start, &End, false, PlayerController);

			//}

		}
	}
}

void APuzzleProjPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("OnResetVR", EInputEvent::IE_Pressed, this, &APuzzleProjPawn::OnResetVR);
	/*PlayerInputComponent->BindAction("TriggerClick", EInputEvent::IE_Pressed, this, &APuzzleProjPawn::TriggerClick);*/
	PlayerInputComponent->BindAction("TriggerClick", EInputEvent::IE_Pressed, this, &APuzzleProjPawn::TriggerClick);

	PlayerInputComponent->BindAction("TriggerIconsHelp", EInputEvent::IE_Pressed, this, &APuzzleProjPawn::TriggerClickIconHelp);
}

void APuzzleProjPawn::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	OutResult.Rotation = FRotator(-90.0f, -90.0f, 0.0f);
}

void APuzzleProjPawn::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APuzzleProjPawn::TriggerClick()
{
	if (CurrentBlockFocus)
	{
		CurrentBlockFocus->HandleClicked();
	}
}
///only whem play in pc 
void APuzzleProjPawn::TriggerClickIconHelp() {

	if (CurrentBlockFocus)
	{
		APuzzleProjPlayerController* PlayerController = Cast<APuzzleProjPlayerController>(GetWorld()->GetFirstPlayerController());
		//bool usehelpIcon = false;
		if (PlayerController) {
			//usehelpIcon = PlayerController->GameOptions.UseHelpIcon;
			if (!PlayerController->GameOptions.UseHelpIcon)
				return;

			if (CurrentBlockFocus->HasQuestion() || (!CurrentBlockFocus->HasFlag())) {
				PlayerController->SetPlayerCursor(EPlayerCursor::FLAG);
				PlayerController->SetLastPlayerCursor(EPlayerCursor::FLAG);
			}
			else {
				PlayerController->SetPlayerCursor(EPlayerCursor::QUEST);
				PlayerController->SetLastPlayerCursor(EPlayerCursor::QUEST);
			}
		}
		CurrentBlockFocus->HandleClicked();

		if (PlayerController)
			PlayerController->SetPlayerCursor(EPlayerCursor::OPEN);

	}
}
void APuzzleProjPawn::TraceForBlock(FVector* Start, FVector* End, bool bDrawDebugHelpers, APuzzleProjPlayerController* PlayerController)
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, *Start, *End, ECC_Visibility);
	if (bDrawDebugHelpers)
	{
		DrawDebugLine(GetWorld(), *Start, HitResult.Location, FColor::Red);
		DrawDebugSolidBox(GetWorld(), HitResult.Location, FVector(20.0f), FColor::Red);
	}
	if (HitResult.Actor.IsValid())
	{
		APuzzleProjBlock* HitBlock = Cast<APuzzleProjBlock>(HitResult.Actor.Get());
		if (CurrentBlockFocus != HitBlock)
		{
			if (CurrentBlockFocus)
			{
				CurrentBlockFocus->Highlight(false, DarkModeOn);
			}
			if (HitBlock)
			{
				HitBlock->Highlight(true, DarkModeOn);
			}
			CurrentBlockFocus = HitBlock;
		}
	}
	else if (CurrentBlockFocus)
	{
		CurrentBlockFocus->Highlight(false, DarkModeOn);
		CurrentBlockFocus = nullptr;
	}
}