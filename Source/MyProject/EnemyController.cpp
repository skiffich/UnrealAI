// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


AEnemyController::AEnemyController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	check(BehaviorTreeComponent);
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
            UE_LOG(LogTemp, Display, TEXT("Start tree"));
            BehaviorTreeComponent->StartTree(*(Enemy->BehaviorTree));
        }
    }
}
