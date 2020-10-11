// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SHADOW_TASK_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//movement functions
	void HorizontalMovement(float value);
	void VerticalMovement(float value);

	//movement rotation functions
	void HorizontalRotation(float value);
	void VerticalRotation(float value);

	//check if player is jumping function
	void CheckJump();

	//camera zoom function
	void Zoom(float value);

	//switch perspectives function
	void Switch();

	//shoot function
	void Shoot();

	//creating bullet sub class 
	UPROPERTY(EditAnywhere, Category = "Shooting")
	TSubclassOf<class ABullet> BulletClass;

	//bool to store if the perspective is first person
	UPROPERTY()
		bool firstPerson;

	//bool to store player jump
	UPROPERTY()
		bool jumping;

	//camera component
	UPROPERTY()
		UCameraComponent* Camera;

	//spring arm component (for 3rd person view)
	UPROPERTY()
		USpringArmComponent* Arm;
};
