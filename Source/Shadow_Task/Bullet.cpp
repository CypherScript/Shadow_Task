// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Target.h"
#include "GameFRamework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create mesh component object and set as root component
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	SetRootComponent(BulletMesh);

	//create projectile movement object and set properties
	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>("BulletMovement");
	BulletMovement->InitialSpeed = 2000.f;
	BulletMovement->MaxSpeed = 2000.f;

	//wrapping (with the on actor hit delegate and add dynamic macro) the on bullet hit method
	OnActorHit.AddDynamic(this, &ABullet::OnBulletHit);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//on bullet hit fuunction definition
void ABullet::OnBulletHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	//check if bullet hits target
	if (ATarget* Target = Cast<ATarget>(OtherActor))
	{
		//destroy the target
		Target->Destroy();
	}
	
	//destroy the bullet
	Destroy();
}

