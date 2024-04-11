// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

UCLASS()
class MYPROJECT_API AEnemyController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyController();

    virtual void OnPossess(APawn* InPawn) override;

    UBlackboardComponent* GetBlackboard();

    void StopBehaviorTree();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

private:
    UPROPERTY(VisibleAnywhere, Category = "AI")
    bool bCanSeePlayer;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    bool bSoundHeard;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    FTimerHandle HearingTimer;


protected:
    UPROPERTY(BlueprintReadWrite, Category = Behavior)
    class UBehaviorTreeComponent* BehaviorTreeComponent;

    UPROPERTY(BlueprintReadWrite, Category = Behavior)
    class UBlackboardComponent* BlackboardComponent;

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "AI")
    class UAISenseConfig_Sight* SightConfig;

    UPROPERTY(VisibleDefaultsOnly, Category = "AI")
    class UAISenseConfig_Hearing* HearingConfig;

public:
    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};

