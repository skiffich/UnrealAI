// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyBaseCharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class MYPROJECT_API AEnemyCharacter : public AMyBaseCharacter
{
    GENERATED_BODY()

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // MakeEditWidget:
    // Shows a wireframe diamond in-world at an offset relative to the parent actor.
    // It also shows some debug text.
    // Used for Transform/Rotator properties. Indicates that the property
    // should be exposed in the viewport as a movable widget.

    UPROPERTY(EditAnywhere, Category = "Patrol", Meta = (MakeEditWidget))
    FVector PatrolPoint1;

    UPROPERTY(EditAnywhere, Category = "Patrol", Meta = (MakeEditWidget))
    FVector PatrolPoint2;

    UPROPERTY(EditAnywhere, Category = "Patrol", Meta = (MakeEditWidget))
    FVector PatrolPoint3;

    UPROPERTY(EditAnywhere, Category = "Patrol", Meta = (MakeEditWidget))
    FVector PatrolPoint4;

    UFUNCTION(BlueprintCallable)
    const FVector& GetNextPatrolLocation();

private:
    TArray<FVector> PatrolPoints;

    int CurrentPatrolIndex = 0;
};
