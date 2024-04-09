// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HealthBar.h"
#include "Components/WidgetComponent.h"


// Sets default values
AMyBaseCharacter::AMyBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCapsule"));
	AttackCapsule->InitCapsuleSize(10.f, 30.f);
	AttackCapsule->CanCharacterStepUpOn = ECB_No;
	AttackCapsule->SetGenerateOverlapEvents(true);
	AttackCapsule->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	AttackCapsule->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LeftHandSocket"));
	AttackCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMyBaseCharacter::OnOverlapBegin_AttackCapsule);
	AttackCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EquippedWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquippedWeapon"));
	BackWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackWeapon"));

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AMyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->GetAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &AMyBaseCharacter::OnMontageBlendEndAttack);

	UHealthBar* HealthBar = Cast<UHealthBar>(HealthWidgetComp->GetUserWidgetObject());
	HealthBar->SetOwnerCharacter(this);
}

void AMyBaseCharacter::OnConstruction(const FTransform& Transform)
{
	EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GunHoldSocketLeft"));
	BackWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("BackWeaponSocket"));
}

// Called every frame
void AMyBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyBaseCharacter::ActivateAttack(bool Activate)
{
	if (Activate)
	{
		AttackCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		AttackCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


void AMyBaseCharacter::Attack()
{
	if (Health <= 0.f) { return; }

	if (bIsAiming) // If aiming -> shoot
	{
		const float kLineTraceDistance = 10000.f;

		// Determine the start and end of the trace
		FVector Start = GetShootStartLocation();
		FVector End = Start + (GetShootRotation().Vector() * kLineTraceDistance);

		// Additional trace parameters
		FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, NULL);
		TraceParams.bTraceComplex = false;
		TraceParams.AddIgnoredActor(this);

		// Re-initialize hit info
		FHitResult HitDetails = FHitResult(ForceInit);

		bool bIsHit = GetWorld()->LineTraceSingleByChannel(
			HitDetails, // FHitResult object that will be populated with hit info
			Start, // Start position
			End, // End position
			ECC_Pawn, // Camera channel
			TraceParams // Additional trace settings
		);

		if (bIsHit) // something was hit
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("We hit something"));
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 5.f, ECC_WorldStatic, 1.f);
			if (HitDetails.GetActor())
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Hit Actor Name: %s"), *HitDetails.GetActor()->GetName()));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("No Actor was hit"));
				DrawDebugBox(GetWorld(), HitDetails.ImpactPoint, FVector(2.f, 2.f, 2.f), FColor::Blue, false, 5.f, ECC_WorldStatic, 1.f);
			}

			UGameplayStatics::ApplyDamage(HitDetails.GetActor(), // Damaged Actor
				Damage, // Damage
				GetController(), // Instigator (Controller)
				this, // Damage Causer (Actor)
				DamageTypeClass); // default damage type
		}
		else // we missed
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Nothing was hit"));
			DrawDebugLine(GetWorld(), Start, End, FColor::Purple, false, 5.f, ECC_WorldStatic, 1.f);
		}
	}
		// If not aiming -> melee attack
	else
	{
		if (AttackMontage)
		{
			if (GetCurrentMontage() == nullptr)
			{
				// Play Animation Montage on the character mesh
				PlayAnimMontage(AttackMontage);

				Aim(false); // Stop Aiming

				EquippedWeapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
				EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("LeftHandBlockSocket"));
			}
			else
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("MyBaseCharacter: AttackMontage already playing"));
				}
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Red, TEXT("MyBaseCharacter: Cannot play AttackMontage"));
			}
		}
	}
}

void AMyBaseCharacter::OnOverlapBegin_AttackCapsule(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor->IsA(ACharacter::StaticClass()))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Hit Other Character"));
		}

		if (DamageTypeClass)
		{
			UGameplayStatics::ApplyDamage(OtherActor, // Damaged Actor
				Damage,    // Damage
				GetController(), // Instigator's Controller
				this,      // Damage Causer (Actor)
				DamageTypeClass); // Damage Type Class
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("AMyBaseCharacter: DamageTypeClass not set!!!"));
			}
		}
	}
}



void AMyBaseCharacter::Aim(bool Aim)
{
	bIsAiming = Aim;
}


bool AMyBaseCharacter::IsAttacking()
{
	UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance && AttackMontage)
	{
		return AnimInstance->Montage_IsPlaying(AttackMontage);
	}
	return false;
}


void AMyBaseCharacter::OnMontageBlendEndAttack(UAnimMontage* animMontage, bool bInterrupted)
{
	if (bInterrupted)
	{
		UE_LOG(LogTemp, Warning, TEXT("MY ANIMATION WAS INTERRUPTED!!!!!!!!!!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MY ANIMATION IS BLENDING OUT!!!!!!!!!!"));

		EquippedWeapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GunHoldSocketLeft"));
	}
}

float AMyBaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("TakeDamage"));
	if (Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
		Die(DamageCauser);
	}
	else
	{
		Health -= DamageAmount;
	}
	return DamageAmount;
}

void AMyBaseCharacter::Die(AActor* Causer)
{
	HealthWidgetComp->SetVisibility(false);

	AttackCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMyBaseCharacter::DeathEnd()
{
	// Function definition presumably to be filled in later
}
