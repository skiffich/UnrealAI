// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


AEnemyController::AEnemyController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	check(BehaviorTreeComponent);

    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
    check(BlackboardComponent);
}

void AEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (InPawn == nullptr)
    {
        return;
    }

    AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(InPawn);
    if (Enemy)
    {
        if (Enemy->BehaviorTree)
        {
            BlackboardComponent->InitializeBlackboard(*(Enemy->BehaviorTree->BlackboardAsset));
            BehaviorTreeComponent->StartTree(*(Enemy->BehaviorTree));
        }
    }
}

UBlackboardComponent* AEnemyController::GetBlackboard()
{
    return BlackboardComponent;
}