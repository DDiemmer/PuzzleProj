// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleProjFlag.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"

// Sets default values
APuzzleProjFlag::APuzzleProjFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	MeshFlag = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshFlag"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Puzzle/Meshes/placa"));
	MeshFlag->SetStaticMesh(MeshAsset.Object);
	
	MeshFlag->BodyInstance.SetCollisionProfileName("NoCollision");

	MeshFlag->SetRelativeScale3D(FVector(7.0f, 7.0f, 9.0f));

	MeshFlag->SetCastShadow(false);

	MeshFlag->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
}



