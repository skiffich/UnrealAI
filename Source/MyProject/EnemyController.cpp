// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "NavigationSystem.h"

void AEnemyController::BeginPlay()
{
    Super::BeginPlay();
    GoToRandomWaypoint();
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

void AEnemyController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);
    GoToRandomWaypoint();
}
