// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyBaseCharacter.h"
#include "MainCharacter.generated.h"

UCLASS()
class MYPROJECT_API AMainCharacter : public AMyBaseCharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** FPS camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FPSCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float AimFOV = 45;

	float StartFOV;
	float DesiredFOV;


public:
	virtual void Aim(bool Aim) override;

public:
	// Sets default values for this character's properties
	AMainCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void DeathEnd() override;
	void SetHealth(float Amount);
	void AddXP(int32 AddedXP) { XP += AddedXP; }

protected:
	virtual FVector GetShootStartLocation() override;
	virtual FRotator GetShootRotation() override;
};
