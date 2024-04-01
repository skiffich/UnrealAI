// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "MyBaseCharacter.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (BaseCharacter == nullptr)
	{
		BaseCharacter = Cast<AMyBaseCharacter>(TryGetPawnOwner());
	}
}

void UMainAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (BaseCharacter == nullptr)
	{
		BaseCharacter = Cast<AMyBaseCharacter>(TryGetPawnOwner());
	}

	if (BaseCharacter)
	{
		bIsCrouching = BaseCharacter->bIsCrouched;
		bIsAiming = BaseCharacter->bIsAiming;

		if (BaseCharacter->IsAttacking())
		{
			// World location of the Right hand socket
			FTransform RightHandIKTransform = BaseCharacter->GetEquippedWeapon()->GetSocketTransform("RightHandBlockIK");
			FRotator RightHandIKTrandformRotation;
			// Convert it to  local space for the left hand, This is done because we’re going to have our right hand move explicitly relative to the left hand, so they stay in sync.
			BaseCharacter->GetMesh()->TransformToBoneSpace("LeftHand", RightHandIKTransform.GetLocation(), FRotator::ZeroRotator, RightHandIKTrandformLocation, RightHandIKTrandformRotation);
		}
		else
		{
			// World location of the Right hand socket
			FTransform RightHandIKTransform = BaseCharacter->GetEquippedWeapon()->GetSocketTransform("RightHandIK");
			FRotator RightHandIKTrandformRotation;
			// Convert it to  local space for the left hand, This is done because we’re going to have our right hand move explicitly relative to the left hand, so they stay in sync.
			BaseCharacter->GetMesh()->TransformToBoneSpace("LeftHand", RightHandIKTransform.GetLocation(), FRotator::ZeroRotator, RightHandIKTrandformLocation, RightHandIKTrandformRotation);
		}

		FVector Speed = BaseCharacter->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0);

		MovementSpeed = LateralSpeed.Size();

		Direction = UKismetAnimationLibrary::CalculateDirection(Speed, BaseCharacter->GetActorRotation());

		bIsInAir = BaseCharacter->GetMovementComponent()->IsFalling();

		FRotator PawnRotation = BaseCharacter->GetActorRotation();
		FRotator ControllerRotation = BaseCharacter->GetControlRotation();
		FRotator DeltaRotation = ControllerRotation - PawnRotation;

		FRotator Interp = FMath::RInterpTo(FRotator(AimPitch, AimYaw, 0), DeltaRotation, DeltaTime, 15);
		AimPitch = FMath::ClampAngle(Interp.Pitch, -90, 90);
		AimYaw = FMath::ClampAngle(Interp.Yaw, -90, 90);

		Health = BaseCharacter->Health;
	}
}
