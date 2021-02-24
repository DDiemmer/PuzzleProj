// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PuzzleProjPlayerController.h"
#include "PuzzleProjPawn.generated.h"

UCLASS(config=Game)
class APuzzleProjPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

protected:
	void OnResetVR();
	void TriggerClick();
	void TriggerClickIconHelp();
	//void TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers, APuzzleProjPlayerController* PlayerController);
	void TraceForBlock(FVector* Start, FVector* End, bool bDrawDebugHelpers, APuzzleProjPlayerController* PlayerController);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class APuzzleProjBlock* CurrentBlockFocus;

private:
	bool DarkModeOn = false;

};
