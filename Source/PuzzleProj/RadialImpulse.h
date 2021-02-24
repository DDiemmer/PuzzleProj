// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RadialImpulse.generated.h"

UCLASS()
class PUZZLEPROJ_API ARadialImpulse : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARadialImpulse();

	void SetExplosion(FVector ExpLocation, float Radius);

protected:
	

public:	
	
};
