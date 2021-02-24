// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleProjFlag.generated.h"

UCLASS()
class PUZZLEPROJ_API APuzzleProjFlag : public AActor
{
	GENERATED_BODY()
	
		UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshFlag;

public:	
	// Sets default values for this actor's properties
	APuzzleProjFlag();

protected:
	

public:	
	UPROPERTY()
		USceneComponent* Root;

};
