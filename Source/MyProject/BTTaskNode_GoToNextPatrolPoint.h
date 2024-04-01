// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_GoToNextPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTTaskNode_GoToNextPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_GoToNextPatrolPoint();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
