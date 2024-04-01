// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_MoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTTaskNode_MoveToTarget : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTaskNode_MoveToTarget();

    UPROPERTY(EditAnywhere)
    FBlackboardKeySelector TargetActorKey;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

