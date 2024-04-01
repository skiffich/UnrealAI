// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


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
}

// Called when the game starts or when spawned
void AMyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->GetAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &AMyBaseCharacter::OnMontageBlendEndAttack);
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
	if (AttackMontage)
	{
		if (GetCurrentMontage() == nullptr)
		{
			// Play Animation Montage on the character mesh
			PlayAnimMontage(AttackMontage);


			Aim(false); // Stop Aiming
		}
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("MyBaseCharacter: AttackMontage already playing"));
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Red, TEXT("MyBaseCharacter: Cannot play AttackMontage"));
	}

	EquippedWeapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("LeftHandBlockSocket"));
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
	AttackCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMyBaseCharacter::DeathEnd()
{
	// Function definition presumably to be filled in later
}
