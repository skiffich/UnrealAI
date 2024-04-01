// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_GoToNextPatrolPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "EnemyCharacter.h"
#include <Navigation/PathFollowingComponent.h>

UBTTaskNode_GoToNextPatrolPoint::UBTTaskNode_GoToNextPatrolPoint()
{
    NodeName = TEXT("GoToNextPatrolPoint");
}

EBTNodeResult::Type UBTTaskNode_GoToNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Make sure the properties are valid
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr) return EBTNodeResult::Failed;

    UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
    if (BlackboardComponent == nullptr) return EBTNodeResult::Failed;

    AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(AIController->GetPawn());
    if (EnemyCharacter == nullptr) return EBTNodeResult::Failed;

    UWorld* World = EnemyCharacter->GetWorld();
    if (World == nullptr) return EBTNodeResult::Failed;

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);
    if (NavSystem == nullptr) return EBTNodeResult::Failed;

    // Move to the next patrol point
    FVector TargetLocation = EnemyCharacter->GetNextPatrolLocation();
    EPathFollowingRequestResult::Type Result = AIController->MoveToLocation(TargetLocation, 10.0f, false);

    // Check the result
    if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
    else if (Result == EPathFollowingRequestResult::Type::Failed)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveToNextPatrolPoint failed for %s"), *EnemyCharacter->GetName());
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }
    else
    {
        AIController->GetPathFollowingComponent()->OnRequestFinished.AddLambda(
            [this, &OwnerComp](FAIRequestID RequestID, const FPathFollowingResult& Result)
        {
            if (Result.Code == EPathFollowingResult::Success)
            {
                // Notify Behavior Tree that the task is completed
                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            }
            else
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
            }
        }
        );
    }

    return EBTNodeResult::InProgress;
}


