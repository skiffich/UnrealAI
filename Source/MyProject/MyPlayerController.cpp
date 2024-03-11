#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "MyBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


// Called when the game starts or when spawned
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	ACharacter* ControlledCharacter = GetCharacter();
	if (ControlledCharacter)
	{
		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

}

// Allows the PlayerController to set up custom input bindings.
void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyPlayerController::StopMove);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Look);

		//Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyPlayerController::StopJumping);

		//Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AMyPlayerController::ToggleSprint);

		//Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMyPlayerController::ToggleCrouch);

		//Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMyPlayerController::Attack);

		//Aim
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AMyPlayerController::ToggleAim);


	}
}

void AMyPlayerController::Move(const FInputActionValue& Value)
{
	if (AMyBaseCharacter* ControlledCharacter = CastChecked<AMyBaseCharacter>(GetCharacter()))
	{

		// input is a Vector2D
		FVector2D MovementVector = Value.Get<FVector2D>();

		ControlledCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

		// find out which camera is facing
		const FRotator Rotation = GetControlRotation();
		const FRotator CameraRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
		// add movement in that direction
		ControlledCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		ControlledCharacter->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyPlayerController::StopMove()
{
	if (ACharacter* ControlledCharacter = CastChecked<ACharacter>(GetCharacter()))
	{
		ControlledCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void AMyPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// add yaw and pitch input to controller
	AddYawInput(LookAxisVector.X);

	//LookAxisVector.Y = FMath::ClampAngle(LookAxisVector.Y, -30, 30);
	AddPitchInput(-LookAxisVector.Y);
}

void AMyPlayerController::Jump()
{
	ACharacter* ControlledCharacter = GetCharacter();
	if (ControlledCharacter)
	{
		ControlledCharacter->Jump();
	}
}

void AMyPlayerController::StopJumping()
{
	ACharacter* ControlledCharacter = GetCharacter();
	if (ControlledCharacter)
	{
		ControlledCharacter->StopJumping();
	}
}

void AMyPlayerController::ToggleSprint()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		if (UCharacterMovementComponent* CharacterMovementComponent = ControlledCharacter->GetCharacterMovement())
		{
			if (CharacterMovementComponent->MaxWalkSpeed == WalkSpeed)
			{
				CharacterMovementComponent->MaxWalkSpeed = RunSpeed;
			}
			else
			{
				CharacterMovementComponent->MaxWalkSpeed = WalkSpeed;
			}
		}
	}
}

void AMyPlayerController::ToggleCrouch()
{
	if (AMyBaseCharacter* ControlledCharacter = CastChecked<AMyBaseCharacter>(GetCharacter()))
	{

		if (!ControlledCharacter->bIsCrouched)
		{
			ControlledCharacter->Crouch();
		}
		else
		{
			ControlledCharacter->UnCrouch();
		}
	}
}

void AMyPlayerController::Attack()
{
	if(AMyBaseCharacter* ControlledCharacter = CastChecked<AMyBaseCharacter>(GetCharacter()))
	{
		ControlledCharacter->Attack();
	}
}


void AMyPlayerController::ToggleAim()
{
	if (AMyBaseCharacter* ControlledCharacter = CastChecked<AMyBaseCharacter>(GetCharacter()))
	{
		if (ControlledCharacter->IsAttacking())
			return;

		if (ControlledCharacter->bIsAiming) // If we are aiming, stop aiming
		{
			ControlledCharacter->Aim(false);
		}
		else // We are not aiming, start aiming
		{

			ControlledCharacter->Aim(true);
		}
	}
}

