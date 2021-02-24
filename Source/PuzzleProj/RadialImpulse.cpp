// Fill out your copyright notice in the Description page of Project Settings.


#include "RadialImpulse.h"
#include "Engine/GameEngine.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Public/WorldCollision.h"
#include "Core/Public/Math/Quat.h"


// Sets default values
ARadialImpulse::ARadialImpulse()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ARadialImpulse::SetExplosion(FVector ExpLocation, float Radius)
{
	const FName TraceTag("SightTrace"); FCollisionQueryParams TraceParams;

	//TraceParams.TraceTag = TraceTag; TraceParams.bTraceAsyncScene = true; TraceParams.bReturnPhysicalMaterial = true; GetWorld()->DebugDrawTraceTag = TraceTag;

	TArray<FHitResult> Hits;

	FVector StartLoc = ExpLocation;


	FVector EndLoc = ExpLocation;
	EndLoc.X += 1.f;

	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(Radius);

	//DrawDebugSphere(GetWorld(), StartLoc, MyColSphere.GetSphereRadius(), 50, FColor::Purple, true);

	bool isHit = GetWorld()->SweepMultiByChannel(Hits, StartLoc, EndLoc, FQuat::Identity, ECollisionChannel::ECC_WorldStatic, MyColSphere, TraceParams);
	if (isHit) {
		for (FHitResult OutHit : Hits)
		{
			//if (GEngine) //requires Engine.h include statement
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Magenta, OutHit.GetActor()->GetName());
			//}

			/*TArray<AActor*> Children = Cast<UStaticMeshComponent>((OutHit.GetActor())->GetAllChildActors(Children));*/

			UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>((OutHit.GetActor())->GetRootComponent());

			if (MeshComp)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("meshCopm"));
				// alternivly you can use  ERadialImpulseFalloff::RIF_Linear for the impulse to get linearly weaker as it gets further from origin.
				// set the float radius to 500 and the float strength to 2000.
				MeshComp->AddRadialImpulse(StartLoc, 500.f, 1500.f, ERadialImpulseFalloff::RIF_Constant, true);
			}
			else {
				///GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("try again"));

				UStaticMeshComponent* MeshChildComp = Cast<UStaticMeshComponent>((OutHit.GetActor())->GetRootComponent()->GetChildComponent(0));

				if (MeshChildComp)
				{

					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("que euee ee"));
					MeshChildComp->AddRadialImpulse(StartLoc, 500.f, 1500.f, ERadialImpulseFalloff::RIF_Constant, true);

				}

			}
		}
	}
}


