// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter.h"

UBTTaskNode_Attack::UBTTaskNode_Attack()
{
    NodeName = TEXT("AttackTargetWhenInRange");
}

EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Make sure the properties are valid
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr) return EBTNodeResult::Failed;
    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (BlackboardComponent == nullptr) return EBTNodeResult::Failed;
    AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(AIController->GetPawn());
    if (EnemyCharacter == nullptr) return EBTNodeResult::Failed;
    UWorld* World = EnemyCharacter->GetWorld();
    if (World == nullptr) return EBTNodeResult::Failed;

    EnemyCharacter->Attack();

    return EBTNodeResult::Succeeded;
}
