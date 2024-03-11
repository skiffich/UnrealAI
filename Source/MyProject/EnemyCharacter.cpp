// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    // We need to get the actor location and add the patrol point to is
    // since these patrol points move with the actor
    PatrolPoints.Add(PatrolPoint1 + GetActorLocation());
    PatrolPoints.Add(PatrolPoint2 + GetActorLocation());
    PatrolPoints.Add(PatrolPoint3 + GetActorLocation());
    PatrolPoints.Add(PatrolPoint4 + GetActorLocation());
}

const FVector& AEnemyCharacter::GetNextPatrolLocation()
{
    if (CurrentPatrolIndex >= PatrolPoints.Num())
    {
        CurrentPatrolIndex = 0;
    }
    return PatrolPoints[CurrentPatrolIndex++];
}
