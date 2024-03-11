// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

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

