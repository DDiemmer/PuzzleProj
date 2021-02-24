// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleProjQuestion.generated.h"

UCLASS()
class PUZZLEPROJ_API APuzzleProjQuestion : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	APuzzleProjQuestion();

	virtual void Tick(float DeltaSeconds) override;

protected:

public:
	UPROPERTY()
		USceneComponent* Root;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshQuestionSign;

	UPROPERTY()
		FRotator RotationRate;

	float SpeedRotation;
};
