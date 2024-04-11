// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo.h"
#include "EnemyCharacter.h"
#include "MyBaseCharacter.h"
#include <Components/SphereComponent.h>
#include <Perception/AIPerceptionStimuliSourceComponent.h>
#include "Perception/AISense_Sight.h"

// Sets default values
AAmmo::AAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create and configure the static mesh component
    AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
    RootComponent = AmmoMesh;

    // Create and configure the collision sphere
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->SetupAttachment(RootComponent);
    CollisionSphere->InitSphereRadius(60.f);
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAmmo::OnOverlapBegin);
}

// Handle overlap
void AAmmo::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor != this && OtherActor != nullptr)
    {
        // Assuming "APlayerCharacter" is your player class
        if (OtherActor->IsA(AMyBaseCharacter::StaticClass()))
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s picked up"), *GetName()));
            UE_LOG(LogTemp, Display, TEXT("%s picked up"), *GetName());

            Destroy();
        }
    }
}