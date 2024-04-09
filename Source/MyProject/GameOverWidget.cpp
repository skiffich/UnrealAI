// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Ensure the button is valid and bind the click event
    if (btn_GameOverConfirm)
    {
        btn_GameOverConfirm->OnClicked.AddDynamic(this, &UGameOverWidget::OnButtonClicked);
    }
}

void UGameOverWidget::OnButtonClicked()
{
    // Close the game when the button is clicked
    if (APlayerController* PC = GetOwningPlayer())
    {
        UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, true);
    }
}