// Fill out your copyright notice in the Description page of Project Settings.


#include "DealDamageAnimNotifyState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MyBaseCharacter.h"


void UDealDamageAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Yellow, TEXT("DealDamage Begin Notify"));

    AMyBaseCharacter* Character = Cast<AMyBaseCharacter>(MeshComp->GetOwner());
    if (Character)
    {
        Character->ActivateAttack(true);
    }
}

void UDealDamageAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
    return;
    FVector StartTraceLocation = MeshComp->GetSocketLocation(TEXT("RightHandSocket"));
    FVector EndTraceLocation = MeshComp->GetSocketLocation(TEXT("LeftHandSocket"));
    float Radius = 10;
    bool bComplexTrace = false;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(MeshComp->GetOwner());
    bool bIgnoreSelf = true;
    FLinearColor TraceColor = FLinearColor::Red;
    FLinearColor TraceHitColor = FLinearColor::Green;
    float DrawTime = 20;

    TArray<FHitResult> HitArray;

    const bool Hit = UKismetSystemLibrary::SphereTraceMulti(MeshComp, StartTraceLocation, EndTraceLocation, Radius,
        UEngineTypes::ConvertToTraceType(ECC_Camera), bComplexTrace, ActorsToIgnore,
        EDrawDebugTrace::ForDuration, HitArray, bIgnoreSelf, TraceColor, TraceHitColor, DrawTime);

    if (Hit)
    {
        for (const FHitResult HitResult : HitArray)
        {
            UGameplayStatics::ApplyDamage(HitResult.GetActor(), // Damaged Actor
                20, // Damage 
                (Cast<APawn>(MeshComp->GetOwner())->GetController()), // Instigator (Controller)
                MeshComp->GetOwner(), // Damage Causer (Actor)
                UDamageType::StaticClass()); // default damage type
        }
    }

}

void UDealDamageAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Yellow, TEXT("DealDamage End Notify"));

    AMyBaseCharacter* Character = Cast<AMyBaseCharacter>(MeshComp->GetOwner());
    if (Character)
    {
        Character->ActivateAttack(false);
    }
}

