// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyBaseCharacter.generated.h"

UCLASS()
class MYPROJECT_API AMyBaseCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Collision component for the attack animation */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* AttackCapsule;

	UFUNCTION()
	void OnOverlapBegin_AttackCapsule(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BackWeapon;

private:
	UFUNCTION()
	virtual void OnMontageBlendEndAttack(UAnimMontage* animMontage, bool bInterrupted);

public:
	// Sets default values for this character's properties
	AMyBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Cannot be in the constructor
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ActivateAttack(bool Activate);

	UStaticMeshComponent* GetEquippedWeapon() { return  EquippedWeapon; }

protected:
	virtual FVector GetShootStartLocation() { return GetActorLocation(); }
	virtual FRotator GetShootRotation() { return GetActorRotation(); }

public:
	/** The attack montage to play */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	class UAnimMontage* AttackMontage;

	virtual void Attack();
	
	/** Boolean for when the character is actively aiming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	bool bIsAiming = false;

	virtual void Aim(bool Aim);

	bool IsAttacking();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	int32 XP;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Die(AActor* Causer);

	UFUNCTION(BlueprintCallable)
	virtual void DeathEnd();

public:
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HealthWidgetComp;

};
