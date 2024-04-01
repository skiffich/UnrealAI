// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckSeePlayer.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UBTService_CheckSeePlayer : public UBTService
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "AI")
    FBlackboardKeySelector PlayerLocationKey;

    UPROPERTY(EditAnywhere, Category = "AI")
    float DetectionRadius = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "AI")
    TSubclassOf<AActor> PlayerClass;

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
    virtual FString GetStaticDescription() const override;
};

