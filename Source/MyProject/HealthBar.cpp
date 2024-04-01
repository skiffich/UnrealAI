// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!OwnerCharacter.IsValid())
        return;

    HealthBar->SetPercent(OwnerCharacter->Health / OwnerCharacter->MaxHealth);

    FNumberFormattingOptions Opts;
    Opts.SetMaximumFractionalDigits(0);
    CurrentHealthLabel->SetText(FText::AsNumber(OwnerCharacter->Health, &Opts));
    MaxHealthLabel->SetText(FText::AsNumber(OwnerCharacter->MaxHealth, &Opts));
}
