// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class SHADOW_TASK_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//mesh property so that the bullet is visible
	UPROPERTY(EditAnywhere, Category = "Components")
		UStaticMeshComponent* BulletMesh;

	//projectlie movement property to handle bullet's projectlie movement
	UPROPERTY(EditAnywhere, Category = "Components")
		class UProjectileMovementComponent* BulletMovement;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//on bullet hit function for when the bullet hits something, it gets destroyed
	UFUNCTION()
	void OnBulletHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

};
