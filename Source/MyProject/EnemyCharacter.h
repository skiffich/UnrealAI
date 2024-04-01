// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyBaseCharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class MYPROJECT_API AEnemyCharacter : public AMyBaseCharacter
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "BehaviorTree")
    class UBehaviorTree* BehaviorTree;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:

    UPROPERTY(EditAnywhere, Category = "Patrol")
    int AmountOfMoves = 3;

    // MakeEditWidget:
    // Shows a wireframe diamond in-world at an offset relative to the parent actor.
    // It also shows some debug text.
    // Used for Transform/Rotator properties. Indicates that the property
    // should be exposed in the viewport as a movable widget.

    UPROPERTY(EditAnywhere, Category = "Patrol", Meta = (MakeEditWidget))
    TArray<FVector> LocalPatrolPoints;

    UFUNCTION(BlueprintCallable)
    const FVector& GetNextPatrolLocation();

    bool IsReachedAllPatrolPoints();
    bool IsReadyToPatrol();

private:
    TArray<FVector> PatrolPoints;

    int CurrentPatrolIndex = 0;
    int CurrentMoveIndex = 0;
};
