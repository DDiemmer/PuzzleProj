// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PuzzleProjBlock.h"
#include "PuzzleProjPlayerController.h"
#include "PuzzleProjBlockGrid.h"
#include "PuzzleProjFlag.h"
#include "PuzzleProjQuestion.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"
#include "Components/TextRenderComponent.h"
#include "RadialImpulse.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "Engine/Public/TimerManager.h"
#include "Engine/GameEngine.h"
#include "EngineMinimal.h"
#include "Engine/StaticMesh.h"
#include "Engine/Classes/Engine/LocalPlayer.h"
#include "Engine/Classes/GameFramework/Actor.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

APuzzleProjBlock::APuzzleProjBlock()
{

	BackBlock = nullptr;
	FrontBlock = nullptr;
	TopBlock = nullptr;
	DownBlock = nullptr;

	FatorMult = 1.f;

	ImpulseRadius = 240.f;

	alreadyVerifyNumBombs = false;

	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> RedBlinkMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OnQuestionMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OnFlagMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OnOpenMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OnOpenMaterialLightOn;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OnQuestionMaterialLightOn;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OnFlagMaterialLightOn;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseHighMaterial.BaseHighMaterial"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/RedMaterial.RedMaterial"))
			, RedBlinkMaterial(TEXT("/Game/Puzzle/Meshes/RedBlinkMaterial_Inst.RedBlinkMaterial_Inst"))
			, OnQuestionMaterial(TEXT("/Game/Puzzle/Meshes/QuestionMaterial.QuestionMaterial"))
			, OnFlagMaterial(TEXT("/Game/Puzzle/Meshes/FlagMaterial.FlagMaterial"))
			, OnOpenMaterial(TEXT("/Game/Puzzle/Meshes/OpenMaterial.OpenMaterial"))
			, OnOpenMaterialLightOn(TEXT("/Game/Puzzle/Meshes/OpenMaterialLightOn.OpenMaterialLightOn"))
			, OnQuestionMaterialLightOn(TEXT("/Game/Puzzle/Meshes/QuestionMaterialLightOn.QuestionMaterialLightOn"))
			, OnFlagMaterialLightOn(TEXT("/Game/Puzzle/Meshes/FlagMaterialLightOn.FlagMaterialLightOn"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	/*DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;*/
	// Create static mesh component
	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	//BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 1.f));
	//BlockMesh->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 128.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	//BlockMesh->AttachTo(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &APuzzleProjBlock::BlockClicked);
	//BlockMesh->OnClicked.AddDynamic(this, &APuzzleProjBlock::BlockClicked);

	BlockMesh->OnInputTouchBegin.AddDynamic(this, &APuzzleProjBlock::OnFingerPressedBlock);
	BlockMesh->OnInputTouchLeave.AddDynamic(this, &APuzzleProjBlock::OnFingerPressedLeaveBlock);
	BlockMesh->OnInputTouchEnd.AddDynamic(this, &APuzzleProjBlock::OnFingerPressedEndBlock);
	

	BlockMesh->SetCastShadow(false);
	//BlockMesh->SetSimulatePhysics(true);
	//BlockMesh->SetNotifyRigidBodyCollision(true);
	//BlockMesh->BodyInstance.SetCollisionProfileName("BloclAllDynamic");

	//BlockMesh->OnComponentHit.AddDynamic(this, &APuzzleProjBlock::OnHit);

	//create text to help 
	NText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NText"));
	NText->SetCastShadow(false);

	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
	RedBlinkMaterial = ConstructorStatics.RedBlinkMaterial.Get();
	OnOpenMaterial = ConstructorStatics.OnOpenMaterial.Get();
	OnQuestionMaterial = ConstructorStatics.OnQuestionMaterial.Get();
	OnFlagMaterial = ConstructorStatics.OnFlagMaterial.Get();
	OnOpenMaterialLightOn = ConstructorStatics.OnOpenMaterialLightOn.Get();
	OnQuestionMaterialLightOn = ConstructorStatics.OnQuestionMaterialLightOn.Get();
	OnFlagMaterialLightOn = ConstructorStatics.OnFlagMaterialLightOn.Get();
}

void APuzzleProjBlock::SetPlayerController(APuzzleProjPlayerController* _PlayerController)
{
	PlayerController = Cast<APuzzleProjPlayerController>(GetWorld()->GetFirstPlayerController());
}

void APuzzleProjBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}

#pragma region TouchEvents
void APuzzleProjBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	int32 bytes = (int32)FingerIndex;
	//APuzzleProjBlock::CenterViewportCursor();

	if (FingerIndex == ETouchIndex::Touch1) {
		TouchBeginHere = true;
	}
	else {
		TouchBeginHere = false;
		PlayerController->SetIgnoreTouch(true);
	}
}
void APuzzleProjBlock::CenterViewportCursor()
{
	if (PlayerController)
	{
		PlayerController->CleanupGameViewport();
	}
}

void APuzzleProjBlock::OnFingerPressedEndBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	int32 bytes = (int32)FingerIndex;
	if (TouchBeginHere && !PlayerController->GetIgnoreTouch()) {

		if (FingerIndex == ETouchIndex::Touch1)
			HandleClicked();
	}
	else if (FingerIndex == ETouchIndex::Touch1) {
		PlayerController->SetIgnoreTouch(false);

	}
	//APuzzleProjBlock::CenterViewportCursor();
	TouchBeginHere = false;
}

void APuzzleProjBlock::OnFingerPressedLeaveBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	TouchBeginHere = false;
	PlayerController->SetIgnoreTouch(true);
}

#pragma endregion

void APuzzleProjBlock::HandleClicked()
{
	// Check we are not already active
	if (!bIsActive)
	{
		if (PlayerController) {

			if (PlayerController->PlayerCursor == EPlayerCursor::OPEN) {

				bIsActive = true;

				if (hasFlag)
				{
					Flag->Destroy();
					PlayerController->SetAddNumBomb(false);
				}
				if (hasQuestion) {
					QuestionSign->Destroy();
				}

				if (isBomb) {
					bIsActive = true;
					if (PlayerController) {
						PlayerController->SetAddNumBomb(true);

						PlayerController->SetGameOver();
					}

					APuzzleProjBlock::SetphysicsAtt();
					APuzzleProjBlock::ActiveTimerExplosion(0.5f);
				}
				else {

					PlayerController->UpBlockToUnlock(1);

					//verify number of bombs around 
					int32  numBombs = APuzzleProjBlock::GetBombsAround(true, true);
					APuzzleProjBlock::SetBlockNumber(numBombs);

					if (!DarkMode)
						BlockMesh->SetMaterial(0, OnOpenMaterial);
					else
						BlockMesh->SetMaterial(0, OnOpenMaterialLightOn);
				}
			}
			else if (PlayerController->PlayerCursor == EPlayerCursor::FLAG) {

				if (hasQuestion)
				{
					QuestionSign->Destroy();
				}

				if (!hasFlag) {
					PlayerController->SetAddNumBomb(true);

					Flag = GetWorld()->SpawnActor<APuzzleProjFlag>(GetActorLocation(), FRotator(0, 90, 0));

					if (Flag && FatorMult != 1) {
						FVector scaleFlag = Flag->GetActorScale3D();
						scaleFlag = scaleFlag * FatorMult;
						Flag->SetActorScale3D(scaleFlag);
					}

					hasQuestion = false;
					hasFlag = true;
					if (!DarkMode)
						BlockMesh->SetMaterial(0, OnFlagMaterial);
					else
						BlockMesh->SetMaterial(0, OnFlagMaterialLightOn);
				}
				else
				{
					hasFlag = false;
					Flag->Destroy();
					PlayerController->SetAddNumBomb(false);
					BlockMesh->SetMaterial(0, BaseMaterial);
				}

			}
			else if (PlayerController->PlayerCursor == EPlayerCursor::QUEST) {


				if (hasFlag) {
					PlayerController->SetAddNumBomb(false);
					///remove flag
					Flag->Destroy();
				}

				if (!hasQuestion) {

					FVector QuestionLocation = GetActorLocation();
					QuestionLocation.Z = (256.f * FatorMult) + 1.f;
					QuestionSign = GetWorld()->SpawnActor<APuzzleProjQuestion>(QuestionLocation, FRotator(0.f, 90.f, 0.f));

					if (QuestionSign && FatorMult != 1) {
						FVector scaleQuestion = QuestionSign->GetActorScale3D();
						scaleQuestion = scaleQuestion * FatorMult;
						QuestionSign->SetActorScale3D(scaleQuestion);
					}

					hasQuestion = true;
					hasFlag = false;
					if (!DarkMode)
						BlockMesh->SetMaterial(0, OnQuestionMaterial);
					else
						BlockMesh->SetMaterial(0, OnQuestionMaterialLightOn);
				}
				else {
					hasQuestion = false;
					QuestionSign->Destroy();
					BlockMesh->SetMaterial(0, BaseMaterial);
				}
			}
		}
		// Tell the Grid
		if (OwningGrid != nullptr)
		{
			OwningGrid->AddScore();
		}

	}
}

void APuzzleProjBlock::Highlight(bool bOn, bool DarkModeOn)
{
	if (DarkModeOn) {
		DarkMode = DarkModeOn;
		if (bOn) {
			if (bIsActive) {
				BlockMesh->SetMaterial(0, OnOpenMaterialLightOn);
			}
			else if (hasFlag) {
				BlockMesh->SetMaterial(0, OnFlagMaterialLightOn);
			}
			else if (hasQuestion) {
				BlockMesh->SetMaterial(0, OnQuestionMaterialLightOn);
			}
		}
		else {
			if (bIsActive) {
				BlockMesh->SetMaterial(0, OnOpenMaterial);
			}
			else if (hasFlag) {
				BlockMesh->SetMaterial(0, OnFlagMaterial);
			}
			else if (hasQuestion) {
				BlockMesh->SetMaterial(0, OnQuestionMaterial);
			}
		}

		if (BackBlock)
			BackBlock->HighlightDarkParents(bOn);

		if (FrontBlock)
			FrontBlock->HighlightDarkParents(bOn);

		if (TopBlock)
			TopBlock->HighlightDarkParents(bOn);

		if (DownBlock)
			DownBlock->HighlightDarkParents(bOn);
	}

	// Do not highlight if the block has already been activated.
	if (bIsActive || hasFlag || hasQuestion)
	{
		return;
	}

	if (bOn)
	{
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
	else
	{
		BlockMesh->SetMaterial(0, BlueMaterial);

	}
}

void APuzzleProjBlock::HighlightDarkParents(bool bOn)
{
	if (bOn) {
		if (bIsActive) {
			BlockMesh->SetMaterial(0, OnOpenMaterialLightOn);
		}
		else
		{
			if (hasFlag)
				BlockMesh->SetMaterial(0, OnFlagMaterialLightOn);
			else if (hasQuestion)
				BlockMesh->SetMaterial(0, OnQuestionMaterialLightOn);
			else
				BlockMesh->SetMaterial(0, BaseMaterial);
		}
	}
	else {
		if (bIsActive) {
			BlockMesh->SetMaterial(0, OnOpenMaterial);
		}
		else
		{
			if (hasFlag)
				BlockMesh->SetMaterial(0, OnFlagMaterial);
			else if (hasQuestion)
				BlockMesh->SetMaterial(0, OnQuestionMaterial);
			else
				BlockMesh->SetMaterial(0, BlueMaterial);
		}
	}
}

void APuzzleProjBlock::SetBomb()
{
	isBomb = true;
	Alreadyused = false;
}

void APuzzleProjBlock::SetBlockNumber(int32 _blockNumber, bool setText)
{
	blockNumber = _blockNumber;
	//if (!isBomb)
	if (setText) {
		FColor ColorText = FColor::Black;

		switch (_blockNumber)
		{
		case 0:
			ColorText = FColor::White;
			break;
		case 1:
			ColorText = FColor::Cyan;
			break;
		case 2:
			ColorText = FColor::Blue;
			break;
		case 3:
			ColorText = FColor::Orange;
			break;
		case 4:
			ColorText = FColor::Red;
			break;
		case 5:
			ColorText = FColor::Red;
			break;
		case 6:
			ColorText = FColor::Red;
			break;
		case 7:
			ColorText = FColor::Red;
			break;
		case 8:
			ColorText = FColor::Black;
			break;
		}

		NText->SetRelativeLocation(FVector(-100.f, -45.f, 128.1f));

		NText->SetRelativeRotation(FRotator(90.f, 180.f, 0.f));

		NText->SetXScale(8);
		NText->SetYScale(8);

		NText->SetTextRenderColor(ColorText);
		NText->SetText(FText::Format(LOCTEXT("ScoreFmt", "{0}"), FText::AsNumber(_blockNumber)));
		NText->AttachToComponent(BlockMesh, FAttachmentTransformRules::KeepRelativeTransform);

	}
	//else
		//NText->SetText(FText::Format(LOCTEXT("ScoreFmt", "B{0}"), FText::AsNumber(_blockNumber)));

}

bool APuzzleProjBlock::GetIsBomb()
{
	return isBomb;
}

int32 APuzzleProjBlock::GetBlockNumber()
{
	return blockNumber;
}

void APuzzleProjBlock::ActiveTimerExplosion(float TimeDelay) {
	Alreadyused = true;

	if (!bIsActive)
		BlockMesh->SetMaterial(0, RedBlinkMaterial);


	bIsActive = true;


	FTimerHandle UnusedHandle;

	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &APuzzleProjBlock::Explode, TimeDelay, false);

}

void APuzzleProjBlock::SetphysicsAtt()
{

	APuzzleProjBlock::PropagatePhysicsAtt(EFSIDE::NONES, false);

}

void APuzzleProjBlock::PropagatePhysicsAtt(EFSIDE SIDE, bool onlyOneDirection)
{

	BlockMesh->SetSimulatePhysics(true);
	BlockMesh->SetNotifyRigidBodyCollision(true);
	////BlockMesh->BodyInstance.SetCollisionProfileName("BloclAllDynamic");
	BlockMesh->OnComponentHit.AddDynamic(this, &APuzzleProjBlock::OnHit);

	//set material just to test 

	///set physics on blocks around 

	if (BackBlock && ((SIDE == EFSIDE::FRONTS || SIDE == EFSIDE::NONES) || (!onlyOneDirection && SIDE == EFSIDE::DOWNS))) {
		BackBlock->PropagatePhysicsAtt(EFSIDE::FRONTS, (SIDE == EFSIDE::FRONTS && onlyOneDirection) || SIDE == EFSIDE::DOWNS);
	}
	//come from back 
	if (FrontBlock && ((SIDE == EFSIDE::BACKS || SIDE == EFSIDE::NONES) || (!onlyOneDirection && SIDE == EFSIDE::TOPS))) {
		FrontBlock->PropagatePhysicsAtt(EFSIDE::BACKS, (SIDE == EFSIDE::BACKS && onlyOneDirection) || SIDE == EFSIDE::TOPS);
	}
	//come from down or back and not one direction
	if (TopBlock && ((SIDE == EFSIDE::DOWNS || SIDE == EFSIDE::NONES) || (!onlyOneDirection && SIDE == EFSIDE::BACKS))) {
		TopBlock->PropagatePhysicsAtt(EFSIDE::DOWNS, (SIDE == EFSIDE::DOWNS && onlyOneDirection) || SIDE == EFSIDE::BACKS);
	}
	if (DownBlock && ((SIDE == EFSIDE::TOPS || SIDE == EFSIDE::NONES) || (!onlyOneDirection && SIDE == EFSIDE::FRONTS))) {
		DownBlock->PropagatePhysicsAtt(EFSIDE::TOPS, (SIDE == EFSIDE::TOPS && onlyOneDirection) || SIDE == EFSIDE::FRONTS);
	}

}

void APuzzleProjBlock::SetScaleBlock(FVector ScaleBlock, float factorMult)
{
	BlockMesh->SetRelativeScale3D(ScaleBlock);
	FVector newLoc = GetActorLocation();
	newLoc.Z = 128.f * factorMult;
	BlockMesh->SetRelativeLocation(newLoc);

	FatorMult = factorMult;

}

void APuzzleProjBlock::Explode()
{
	ARadialImpulse* RadiusImpulse = GetWorld()->SpawnActor<ARadialImpulse>(GetActorLocation(), FRotator(0, 0, 0));

	FVector locationExp = GetActorLocation();
	locationExp.Z = -15.f;

	RadiusImpulse->SetExplosion(locationExp, ImpulseRadius * FatorMult);

	BlockMesh->SetMaterial(0, OrangeMaterial);

}

void APuzzleProjBlock::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComponent != NULL) && this->isBomb && !this->Alreadyused) {

		if (this->GetClass() == OtherActor->GetClass()) {

			APuzzleProjBlock::ActiveTimerExplosion(1.f);
		}
	}

}

void APuzzleProjBlock::SetBackBlock(APuzzleProjBlock* _BackBlock)
{
	if (BackBlock == nullptr) {
		BackBlock = _BackBlock;

		if (BackBlock->FrontBlock == nullptr) {
			BackBlock->SetFrontBlock(this);
		}
	}

}

void APuzzleProjBlock::SetFrontBlock(APuzzleProjBlock* _FrontBlock)
{
	if (FrontBlock == nullptr)
		FrontBlock = _FrontBlock;
}

void APuzzleProjBlock::SetTopBlock(APuzzleProjBlock* _TopBlock)
{
	if (TopBlock == nullptr) {
		TopBlock = _TopBlock;

		if (TopBlock->DownBlock == nullptr) {
			TopBlock->SetDownBlock(this);
		}
	}
}

bool APuzzleProjBlock::HasFlag()
{
	return hasFlag;
}

bool APuzzleProjBlock::HasQuestion()
{
	return hasQuestion;
}

void APuzzleProjBlock::SetDownBlock(APuzzleProjBlock* _DownBlock)
{
	if (DownBlock == nullptr)
		DownBlock = _DownBlock;
}

int32 APuzzleProjBlock::GetBombsAround(bool VerifyBackFront, bool VerifytopDown, bool SetNumberInZeroBombs, EFSIDE SIDE)
{
	if (alreadyVerifyNumBombs && VerifytopDown && VerifyBackFront)
		return NumBombsAround;

	int32 bombsAround = 0;

	if (VerifyBackFront) {
		if (BackBlock)
			if (BackBlock->GetIsBomb()) { bombsAround++; }

		if (FrontBlock)
			if (FrontBlock->GetIsBomb()) { bombsAround++; }
	}
	if (VerifytopDown) {

		if (TopBlock)
		{
			bombsAround += TopBlock->GetBombsAround(true, false);
			if (TopBlock->GetIsBomb())
				bombsAround++;
		}
		if (DownBlock)
		{
			bombsAround += DownBlock->GetBombsAround(true, false);
			if (DownBlock->GetIsBomb())
				bombsAround++;
		}
	}

	if (VerifyBackFront && VerifytopDown) {
		alreadyVerifyNumBombs = true;
		NumBombsAround = bombsAround;

		if (bombsAround == 0) {
			if (BackBlock && SIDE != EFSIDE::BACKS) {
				BackBlock->GetBombsAround(true, true, true, EFSIDE::FRONTS);
			}
			if (FrontBlock && SIDE != EFSIDE::FRONTS) {
				FrontBlock->GetBombsAround(true, true, true, EFSIDE::BACKS);
			}
			if (TopBlock && SIDE != EFSIDE::TOPS) {
				TopBlock->GetBombsAround(true, true, true, EFSIDE::DOWNS);
			}
			if (DownBlock && SIDE != EFSIDE::DOWNS) {
				DownBlock->GetBombsAround(true, true, true, EFSIDE::TOPS);
			}
		}
	}

	if (SetNumberInZeroBombs)
	{
		if (hasFlag) {
			Flag->Destroy();
			PlayerController->SetAddNumBomb(false);
		}
		if (hasQuestion) {
			QuestionSign->Destroy();
		}

		APuzzleProjBlock::SetBlockNumber(bombsAround);
		BlockMesh->SetMaterial(0, OnOpenMaterial);
		bIsActive = true;
		//Add Player NumBLockunlock 
		if (PlayerController)
			PlayerController->UpBlockToUnlock(1);

		// nao fez as diagonais 
		if (bombsAround > 0 && SIDE != EFSIDE::NONES)
		{
			if (SIDE == EFSIDE::BACKS || SIDE == EFSIDE::FRONTS)
			{
				//top//down
				if (TopBlock) {
					if (!TopBlock->bIsActive) {
						TopBlock->GetBombsAround(true, true, true, EFSIDE::NONES);
					}
				}
				if (DownBlock) {
					if (!DownBlock->bIsActive) {
						DownBlock->GetBombsAround(true, true, true, EFSIDE::NONES);
					}
				}

			}
			else if (SIDE == EFSIDE::TOPS || SIDE == EFSIDE::DOWNS)
			{
				//Front//Back
				if (FrontBlock) {
					if (!FrontBlock->bIsActive) {
						FrontBlock->GetBombsAround(true, true, true, EFSIDE::NONES);
					}
				}
				if (BackBlock) {
					if (!BackBlock->bIsActive) {
						BackBlock->GetBombsAround(true, true, true, EFSIDE::NONES);
					}
				}
			}

		}
	}

	return bombsAround;
}


#undef LOCTEXT_NAMESPACE