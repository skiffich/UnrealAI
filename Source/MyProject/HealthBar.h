// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyBaseCharacter.h"
#include "HealthBar.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UHealthBar : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetOwnerCharacter(AMyBaseCharacter* InCharacter) { OwnerCharacter = InCharacter; }

protected:
    void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    TWeakObjectPtr<AMyBaseCharacter> OwnerCharacter;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CurrentHealthLabel;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MaxHealthLabel;
};

