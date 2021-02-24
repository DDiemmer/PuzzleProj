// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleProjQuestion.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"

// Sets default values
APuzzleProjQuestion::APuzzleProjQuestion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	MeshQuestionSign = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshQuestionSign"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Puzzle/Meshes/QustionSign"));
	MeshQuestionSign->SetStaticMesh(MeshAsset.Object);

	MeshQuestionSign->BodyInstance.SetCollisionProfileName("NoCollision");

	MeshQuestionSign->SetRelativeScale3D(FVector(12.0f, 12.0f, 7.0f));

	MeshQuestionSign->SetCastShadow(false);

	MeshQuestionSign->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	RotationRate = FRotator(0.f, 40.f, 0.f);
	
	SpeedRotation = 1.f;
}


void APuzzleProjQuestion::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AddActorLocalRotation(RotationRate * DeltaSeconds * SpeedRotation);

}