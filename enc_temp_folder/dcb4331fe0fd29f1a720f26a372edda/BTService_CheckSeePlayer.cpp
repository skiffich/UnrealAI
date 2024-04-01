// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckSeePlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void UBTService_CheckSeePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp == nullptr) return;

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr) return;

    APawn* AIPawn = AIController->GetPawn();
    if (AIPawn == nullptr) return;

    if (PlayerClass && PlayerLocationKey.IsSet())
    {
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerClass, FoundActors);

        if (FoundActors.Num() > 0)
        {
            const FVector& PlayerLocation = FoundActors[0]->GetActorLocation();
            const FVector& AIPawnLocation = AIPawn->GetActorLocation();

            const float Distance = FVector::Distance(PlayerLocation, AIPawnLocation);
            if (Distance < DetectionRadius)
            {
                BlackboardComp->SetValueAsVector(PlayerLocationKey.SelectedKeyName, PlayerLocation);
            }
            else
            {
                BlackboardComp->ClearValue(PlayerLocationKey.SelectedKeyName);
            }
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

FString UBTService_CheckSeePlayer::GetStaticDescription() const
{
    return FString::Printf(TEXT("\n%s: '%s'"), TEXT("Player Class"), *GetNameSafe(PlayerClass))
        .Append(FString::Printf(TEXT("\n%s: '%s'"), TEXT("Player Location Key"), *PlayerLocationKey.SelectedKeyName.ToString()))
        .Append(FString::Printf(TEXT("\n%s: '%s'"), TEXT("Detection Radius"), *FString::SanitizeFloat(DetectionRadius)));
}


