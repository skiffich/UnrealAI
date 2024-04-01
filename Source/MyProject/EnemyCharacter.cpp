// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyCharacter::AEnemyCharacter()
    : Super()
{
    PrimaryActorTick.bCanEverTick = true;

    AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
    AgroSphere->SetupAttachment(GetRootComponent());

    AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
    AttackSphere->SetupAttachment(GetRootComponent());

    ShootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ShootSphere"));
    ShootSphere->SetupAttachment(GetRootComponent());

    Health = 20.f;
    MaxHealth = 20.f;
    XP = 10;
    Damage = 20.f;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    // We need to get the actor location and add the patrol point to is
    // since these patrol points move with the actor
    for (auto& PatrolPoint : LocalPatrolPoints)
    {
        PatrolPoints.Add(PatrolPoint + GetActorLocation());
    }

    AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::AgroSphereBeginOverlap);
    AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::AgroSphereEndOverlap);

    AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::AttackSphereBeginOverlap);
    AttackSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::AttackSphereEndOverlap);

    ShootSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::ShootSphereBeginOverlap);
    ShootSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::ShootSphereEndOverlap);
}

const FVector& AEnemyCharacter::GetNextPatrolLocation()
{
    if (PatrolPoints.Num() > 0)
    {
        if (CurrentPatrolIndex >= PatrolPoints.Num())
        {
            CurrentPatrolIndex = 0;
        }
        return PatrolPoints[CurrentPatrolIndex++];
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("No Patrol Location set for Enemy Actor"));
        return FVector::ZeroVector;
    }
}

bool AEnemyCharacter::IsReachedAllPatrolPoints()
{
    return (PatrolPoints.Num() > 0 && CurrentPatrolIndex >= PatrolPoints.Num());
}

bool AEnemyCharacter::IsReadyToPatrol()
{
    if (CurrentMoveIndex++ >= AmountOfMoves)
    {
        CurrentMoveIndex = 0;
        CurrentPatrolIndex = 0;
        return true;
    }
    return false;
}

void AEnemyCharacter::DeathEnd()
{
    Super::DeathEnd();
    //Destroy();
}

void AEnemyCharacter::Die(AActor* Causer)
{
    Super::Die(Causer);

    AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    AttackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (AEnemyController* EnemyController = Cast<AEnemyController>(GetController()))
    {
        EnemyController->StopBehaviorTree();
    }

    if (AMainCharacter* Main = Cast<AMainCharacter>(Causer))
    {
        Main->AddXP(XP);
    }
}


void AEnemyCharacter::AgroSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        if (AMainCharacter* Main = Cast<AMainCharacter>(OtherActor))
        {
            if (AEnemyController* EnemyController = Cast<AEnemyController>(GetController()))
            {
                EnemyController->GetBlackboard()->SetValueAsObject(TEXT("TargetActor"), Main);
            }
        }
    }
}

void AEnemyCharacter::AgroSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor)
    {
        if (AMainCharacter* Main = Cast<AMainCharacter>(OtherActor))
        {
            if (AEnemyController* EnemyController = Cast<AEnemyController>(GetController()))
            {
                EnemyController->GetBlackboard()->ClearValue(TEXT("TargetActor"));
            }
        }
    }
}

void AEnemyCharacter::AttackSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        if (AMainCharacter* Main = Cast<AMainCharacter>(OtherActor))
        {
            if (AEnemyController* EnemyController = Cast<AEnemyController>(GetController()))
            {
                EnemyController->GetBlackboard()->SetValueAsBool(TEXT("InAttackRange"), true);
            }
        }
    }
}

void AEnemyCharacter::AttackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor)
    {
        if (AMainCharacter* Main = Cast<AMainCharacter>(OtherActor))
        {
            if (AEnemyController* EnemyController = Cast<AEnemyController>(GetController()))
            {
                EnemyController->GetBlackboard()->SetValueAsBool(TEXT("InAttackRange"), false);
            }
        }
    }
}

void AEnemyCharacter::ShootSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        if (AMainCharacter* Main = Cast<AMainCharacter>(OtherActor))
        {
            if (AEnemyController* EnemyController = Cast<AEnemyController>(GetController()))
            {
                EnemyController->GetBlackboard()->SetValueAsObject(TEXT("ShootActor"), Main);
            }
        }
    }
}

void AEnemyCharacter::ShootSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor)
    {
        if (AMainCharacter* Main = Cast<AMainCharacter>(OtherActor))
        {
            if (AEnemyController* EnemyController = Cast<AEnemyController>(GetController()))
            {
                EnemyController->GetBlackboard()->ClearValue(TEXT("ShootActor"));
            }
        }
    }
}

