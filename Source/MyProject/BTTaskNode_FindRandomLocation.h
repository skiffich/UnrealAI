// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_FindRandomLocation.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UBTTaskNode_FindRandomLocation : public UBTTaskNode
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector DestinationVectorKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    float SearchRadius = 300.0f;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

