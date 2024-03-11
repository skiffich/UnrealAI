// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "NavigationSystem.h"
#include "EnemyCharacter.h"

void AEnemyController::BeginPlay()
{
    Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(RandomWaypointTimerHandle, this, &AEnemyController::GoToNextPatrolPoint, 3, false);
}

void AEnemyController::GoToRandomWaypoint()
{
    APawn* ControllingPawn = GetPawn();
    if (ControllingPawn)
    {
        UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
        if (NavSys)
        {
            FVector Result;
            bool bSuccess = NavSys->K2_GetRandomReachablePointInRadius(GetWorld(),
                ControllingPawn->GetActorLocation(),
                Result,
                3000); // Radius
            if (bSuccess)
            {
                MoveToLocation(Result);
            }
        }
    }
}

void AEnemyController::GoToNextPatrolPoint()
{
    AEnemyCharacter* ControllingEnemy = Cast<AEnemyCharacter>(GetPawn());
    if (ControllingEnemy)
    {
        if (!ControllingEnemy->IsReachedAllPatrolPoints())
        {
            UE_LOG(LogTemp, Display, TEXT("Go to NextPatrolLocation"));
            MoveToLocation(ControllingEnemy->GetNextPatrolLocation());
        }
        else if (!ControllingEnemy->IsReadyToPatrol())
        {
            UE_LOG(LogTemp, Display, TEXT("Go to RandomWaypoint"));
            GoToRandomWaypoint();
        }
        else
        {
            UE_LOG(LogTemp, Display, TEXT("Go to NextPatrolLocation"));
            MoveToLocation(ControllingEnemy->GetNextPatrolLocation());
        }
    }
}

void AEnemyController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);
    GetWorld()->GetTimerManager().SetTimer(RandomWaypointTimerHandle, this, &AEnemyController::GoToNextPatrolPoint, 3, false);
}
