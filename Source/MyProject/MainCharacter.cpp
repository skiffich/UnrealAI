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

	Health = 85.f;
	MaxHealth = 100.f;
	Damage = 10.f;
	XP = 0;
}

void AMainCharacter::Aim(bool Aim)
{
	if (Health <= 0.f)
	{
		return;
	}

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

void AMainCharacter::SetHealth(float Amount)
{
	if (Amount > MaxHealth)
	{
		Health = MaxHealth;
	}
	else if (Amount < 0)
	{
		Health = 0;
	}
	else
	{
		Health = Amount;
	}
}

void AMainCharacter::DeathEnd()
{
	Super::DeathEnd();
	UKismetSystemLibrary::QuitGame(this, Cast<APlayerController>(GetController()), EQuitPreference::Quit, true);
}

FVector AMainCharacter::GetShootStartLocation()
{
	return FollowCamera->GetComponentLocation();
}

FRotator AMainCharacter::GetShootRotation()
{
	return FollowCamera->GetComponentRotation();
}
