// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Aim.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_Aim::UBTTask_Aim()
{
    NodeName = TEXT("Aim");
}

EBTNodeResult::Type UBTTask_Aim::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    if (EnemyCharacter->Health > 0)
    {
        EnemyCharacter->Aim(!EnemyCharacter->bIsAiming);

        // Get Player Character
        ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
        if (PlayerCharacter == nullptr) return EBTNodeResult::Failed;

        FVector EnemyLocation = EnemyCharacter->GetActorLocation();
        FRotator EnemyRotation = EnemyCharacter->GetActorRotation();
        FVector PlayerLocation = PlayerCharacter->GetActorLocation();

        // Find look at rotation
        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(EnemyLocation, PlayerLocation);

        // Set the rotation
        EnemyCharacter->SetActorRotation(LookAtRotation);
    }
    else
    {
        // Stop aiming
        EnemyCharacter->Aim(false);
        return EBTNodeResult::Failed;
    }

    return EBTNodeResult::Succeeded;
}
