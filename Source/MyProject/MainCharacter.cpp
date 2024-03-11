// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;  // Face the direction we are moving
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);  // Rotate at this rate
	GetCharacterMovement()->bUseControllerDesiredRotation = true;  // Use controller's rotation

	StartFOV = FollowCamera->FieldOfView;
	DesiredFOV = StartFOV;
}


void AMainCharacter::Attack()
{
	if (bIsAiming) // if aiming -> shoot
	{
		const float kLineTraceDistance = 10000.f;

		// get the camera view
		FVector CameraLoc = FollowCamera->GetComponentLocation();
		FRotator CameraRot = FollowCamera->GetComponentRotation();

		// Determine the start and end of the trace
		FVector Start = CameraLoc;
		FVector End = CameraLoc + (CameraRot.Vector() * kLineTraceDistance);

		// additional trace parameters
		FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, NULL);
		TraceParams.bTraceComplex = false;

		//Re-initialize hit info
		FHitResult HitDetails = FHitResult(ForceInit);

		bool bIsHit = GetWorld()->LineTraceSingleByChannel(
			HitDetails,				// FHitResult object that will be populated with hit info
			Start,					// starting position
			End,					// end position
			ECC_Camera,				// Camera channel
			TraceParams				// additional trace settings
		);

		if (bIsHit) // something was hit
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("We hit something"));
			// start to end, green, will lines always stay on, depth priority, thickness of line
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 5.f, ECC_WorldStatic, 1.f);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Hit Actor Name: %s"), *HitDetails.GetActor()->GetName()));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Distance: %s"), *FString::SanitizeFloat(HitDetails.Distance)));
			DrawDebugBox(GetWorld(), HitDetails.ImpactPoint, FVector(2.f, 2.f, 2.f), FColor::Blue, false, 5.f, ECC_WorldStatic, 1.f);

			UGameplayStatics::ApplyDamage(HitDetails.GetActor(), // Damaged Actor
				100, // Damage 
				GetController(), // Instigator (Controller)
				this, // Damage Causer (Actor)
				UDamageType::StaticClass()); // default damage type
		}
		else // we missed
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Nothing was hit"));
			// start to end, purple, will lines always stay on, depth priority, thickness of line
			DrawDebugLine(GetWorld(), Start, End, FColor::Purple, false, 5.f, ECC_WorldStatic, 1.f);
		}
	}
	else
	{
		Super::Attack();
	}
}

void AMainCharacter::Aim(bool Aim)
{
	Super::Aim(Aim);
	bUseControllerRotationYaw = Aim;
	DesiredFOV = (Aim ? AimFOV : StartFOV);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMin = -30;
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMax = 30;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float FOV = FMath::Lerp(FollowCamera->FieldOfView, DesiredFOV, DeltaTime * 10);
	FollowCamera->SetFieldOfView(FOV);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}