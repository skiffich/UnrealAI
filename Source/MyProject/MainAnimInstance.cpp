// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "MainCharacter.h"




void UMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningPawn == nullptr)
	{
		OwningPawn = TryGetPawnOwner();

		if (OwningPawn)
		{
			MainCharacter = Cast<AMainCharacter>(OwningPawn);
		}

	}
}

void UMainAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (OwningPawn == nullptr)
	{
		OwningPawn = TryGetPawnOwner();
	}

	if (OwningPawn)
	{
		if (MainCharacter == nullptr)
		{
			MainCharacter = Cast<AMainCharacter>(OwningPawn);
		}

		if (MainCharacter)
		{
			bIsCrouching = MainCharacter->bIsCrouched;
			bIsAiming = MainCharacter->bIsAiming;

			if (MainCharacter->IsAttacking())
			{
				// World location of the Right hand socket
				FTransform RightHandIKTransform = MainCharacter->GetEquippedWeapon()->GetSocketTransform("RightHandBlockIK");
				FRotator RightHandIKTrandformRotation;
				// Convert it to  local space for the left hand, This is done because we’re going to have our right hand move explicitly relative to the left hand, so they stay in sync.
				MainCharacter->GetMesh()->TransformToBoneSpace("LeftHand", RightHandIKTransform.GetLocation(), FRotator::ZeroRotator, RightHandIKTrandformLocation, RightHandIKTrandformRotation);
			}
			else
			{
				// World location of the Right hand socket
				FTransform RightHandIKTransform = MainCharacter->GetEquippedWeapon()->GetSocketTransform("RightHandIK");
				FRotator RightHandIKTrandformRotation;
				// Convert it to  local space for the left hand, This is done because we’re going to have our right hand move explicitly relative to the left hand, so they stay in sync.
				MainCharacter->GetMesh()->TransformToBoneSpace("LeftHand", RightHandIKTransform.GetLocation(), FRotator::ZeroRotator, RightHandIKTrandformLocation, RightHandIKTrandformRotation);
			}
		}


		FVector Speed = OwningPawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0);

		MovementSpeed = LateralSpeed.Size();

		Direction = UKismetAnimationLibrary::CalculateDirection(Speed, OwningPawn->GetActorRotation());

		bIsInAir = OwningPawn->GetMovementComponent()->IsFalling();

		FRotator PawnRotation = OwningPawn->GetActorRotation();
		FRotator ControllerRotation = OwningPawn->GetControlRotation();
		FRotator DeltaRotation = ControllerRotation - PawnRotation;

		FRotator Interp = FMath::RInterpTo(FRotator(AimPitch, AimYaw, 0), DeltaRotation, DeltaTime, 15);
		AimPitch = FMath::ClampAngle(Interp.Pitch, -90, 90);
		AimYaw = FMath::ClampAngle(Interp.Yaw, -90, 90);

	}
}
