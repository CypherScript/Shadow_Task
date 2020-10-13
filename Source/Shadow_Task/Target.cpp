// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"

// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create the mesh component and set it as the root component
	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>("TargetMesh");
	SetRootComponent(TargetMesh);

	//give it a starting value
	MoveScale = 200.f;
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();
	
	//assign random value to move direction; done in begin play function so as it to be completely random every time the game starts
	MoveDirection = FMath::VRand();
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//the movement of the target; add the time in seconds to the time since the last frame (delta time) and substracting it to the time in seconds gives you a value between 1 and -1 (sin)
	//then multiply by the direction, and by the scalar
	FVector MoveDelta = (MoveDirection * (FMath::Sin(GetWorld()->GetTimeSeconds() + DeltaTime) - FMath::Sin(GetWorld()->GetTimeSeconds()))) * MoveScale;

	//moves the target and makes sure to not go through walls 
	AddActorWorldOffset(MoveDelta, true);
}

