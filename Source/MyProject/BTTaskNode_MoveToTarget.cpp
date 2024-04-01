// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_MoveToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "EnemyCharacter.h"
#include <Navigation/PathFollowingComponent.h>

UBTTaskNode_MoveToTarget::UBTTaskNode_MoveToTarget()
{
    NodeName = TEXT("MoveToTarget");
}

EBTNodeResult::Type UBTTaskNode_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Make sure the properties are valid
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr) { return EBTNodeResult::Failed; }
    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (BlackboardComponent == nullptr) { return EBTNodeResult::Failed; }
    AActor* EnemyCharacter = Cast<AActor>(AIController->GetPawn());
    if (EnemyCharacter == nullptr) { return EBTNodeResult::Failed; }
    UWorld* World = EnemyCharacter->GetWorld();
    if (World == nullptr) { return EBTNodeResult::Failed; }
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);
    if (NavSystem == nullptr) { return EBTNodeResult::Failed; }

    // Get the target actor
    AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetActorKey.SelectedKeyName));
    if (TargetActor == nullptr) { return EBTNodeResult::Failed; }

    // Move to the target actor
    EPathFollowingRequestResult::Type Result = AIController->MoveToActor(TargetActor, 50.0f, false);

        // Check the result
    if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
    else if (Result == EPathFollowingRequestResult::Type::Failed)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveToTarget failed for %s"), *EnemyCharacter->GetName());
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }
    else
    {
        AIController->GetPathFollowingComponent()->OnRequestFinished.AddLambda(
            [&](FAIRequestID RequestID, const FPathFollowingResult& Result)
            {
                if (Result.Code == EPathFollowingResult::Success)
                {
                    // Notify Behavior Tree that the task is completed
                    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                }
                else if (Result.Code == EPathFollowingResult::Aborted)
                {
                    FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("MoveToTarget failed for %s"), *OwnerComp.GetAIOwner()->GetPawn()->GetName());
                    FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                }
            }
        );
    }

    return EBTNodeResult::InProgress;
}

