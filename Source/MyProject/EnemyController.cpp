// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISenseConfig_Hearing.h"

#include "MainCharacter.h"


AEnemyController::AEnemyController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	check(BehaviorTreeComponent);

    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
    check(BlackboardComponent);

    // Initialize the AI Perception component
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

    // Create the senses
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("Sight Config"));
    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(FName("Hearing Config"));

    // Configuring the sight sense
    SightConfig->SightRadius = 600.0f;
    SightConfig->LoseSightRadius = 700.0f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    // Configuring the hearing sense
    HearingConfig->HearingRange = 900.0f;
    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

    // Assigning the sight and hearing senses to the AI Perception component
    PerceptionComponent->ConfigureSense(*SightConfig);
    PerceptionComponent->ConfigureSense(*HearingConfig);
    PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

}

void AEnemyController::BeginPlay()
{
    Super::BeginPlay();

    // Register the OnTargetPerceptionUpdated function to the OnTargetPerceptionUpdated event
    PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetPerceptionUpdated);
}


void AEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (InPawn == nullptr)
    {
        return;
    }

    AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(InPawn);
    if (Enemy)
    {
        if (Enemy->BehaviorTree)
        {
            BlackboardComponent->InitializeBlackboard(*(Enemy->BehaviorTree->BlackboardAsset));
            BehaviorTreeComponent->StartTree(*(Enemy->BehaviorTree));
        }
    }
}

UBlackboardComponent* AEnemyController::GetBlackboard()
{
    return BlackboardComponent;
}

void AEnemyController::StopBehaviorTree()
{
    BehaviorTreeComponent->StopTree();
}

void AEnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    // Check if the actor is valid
    AActor* SensedActor = Actor;
    if (SensedActor == nullptr) return;

    // Check if the stimulus is sight
    if (Stimulus.Type == SightConfig->GetSenseID())
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            // Check if the sensed actor is the player
            if (Cast<AMainCharacter>(SensedActor))
            {
                bCanSeePlayer = true;
            }
        }
        else
        {
            // Check if the sensed actor is the player
            if (Cast<AMainCharacter>(SensedActor))
            {
                bCanSeePlayer = false;
            }
        }
    }

    // Check if the stimulus is hearing
    if (Stimulus.Type == HearingConfig->GetSenseID())
    {
        // Check if the sound was heard
        if (Stimulus.WasSuccessfullySensed())
        {
            // Check if the sensed actor is the player
            if (Cast<AMainCharacter>(SensedActor))
            {
                bSoundHeard = true;
            }
        }
    }
}

void AEnemyController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bCanSeePlayer)
    {
        GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Red, TEXT("I CAN SEE YOU"));
    }

    if (bSoundHeard)
    {
        GEngine->AddOnScreenDebugMessage(2, 0.1f, FColor::Orange, TEXT("WHAT WAS THAT"));

        // Set the sound heard value to false after 2 seconds
        if (!GetWorld()->GetTimerManager().IsTimerActive(HearingTimer))
        {
            GetWorld()->GetTimerManager().SetTimer(HearingTimer, [this]() { bSoundHeard = false; }, 2.0f, false);
        }
    }
}
