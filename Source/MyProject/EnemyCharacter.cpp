// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyCharacter::AEnemyCharacter()
    : Super()
{
    PrimaryActorTick.bCanEverTick = true;

    AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
    AgroSphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    // We need to get the actor location and add the patrol point to is
    // since these patrol points move with the actor
    for (auto& PatrolPoint : LocalPatrolPoints)
    {
        PatrolPoints.Add(PatrolPoint + GetActorLocation());
    }

    AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::AgroSphereBeginOverlap);
    AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::AgroSphereEndOverlap);
}

const FVector& AEnemyCharacter::GetNextPatrolLocation()
{
    if (PatrolPoints.Num() > 0)
    {
        if (CurrentPatrolIndex >= PatrolPoints.Num())
        {
            CurrentPatrolIndex = 0;
        }
        return PatrolPoints[CurrentPatrolIndex++];
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("No Patrol Location set for Enemy Actor"));
        return FVector::ZeroVector;
    }
}

bool AEnemyCharacter::IsReachedAllPatrolPoints()
{
    return (PatrolPoints.Num() > 0 && CurrentPatrolIndex >= PatrolPoints.Num());
}

bool AEnemyCharacter::IsReadyToPatrol()
{
    if (CurrentMoveIndex++ >= AmountOfMoves)
    {
        CurrentMoveIndex = 0;
        CurrentPatrolIndex = 0;
        return true;
    }
    return false;
}

void AEnemyCharacter::AgroSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        if (AMainCharacter* Main = Cast<AMainCharacter>(OtherActor))
        {
            if (AEnemyController* EnemyController = Cast<AEnemyController>(GetController()))
            {
                EnemyController->GetBlackboard()->SetValueAsObject(TEXT("TargetActor"), Main);
            }
        }
    }
}

void AEnemyCharacter::AgroSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor)
    {
        if (AMainCharacter* Main = Cast<AMainCharacter>(OtherActor))
        {
            if (AEnemyController* EnemyController = Cast<AEnemyController>(GetController()))
            {
                EnemyController->GetBlackboard()->ClearValue(TEXT("TargetActor"));
            }
        }
    }
}
