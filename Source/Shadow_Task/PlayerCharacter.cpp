// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Bullet.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//to tell the engine which character is selected
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//to not use the engine's controller rotation (if true, it won't follow my rotation input)
	bUseControllerRotationYaw = false;

	//create the camera component and attaching to root component
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	//Camera->AttachTo(RootComponent);	

	//create the arm component and attach to root component
	Arm = CreateDefaultSubobject<USpringArmComponent>("Arm");
	Arm->AttachTo(RootComponent);

	//the arm's radius around the player and it's rotation
	Arm->TargetArmLength = 300.f;
	Arm->SetRelativeRotation(FRotator (-45.f, 0.f, 0.f));

	//camera lag
	Arm->bEnableCameraLag = true;
	Arm->CameraLagSpeed = 2;	//controls how fast the camera reaches its target
	Arm->CameraLagMaxDistance = 1.5f;	//max distance the camera can lag behind the current location

	//camera lag rotations
	Arm->bEnableCameraRotationLag = true;
	Arm->CameraRotationLagSpeed = 4;
	Arm->CameraLagMaxTimeStep = 1;

	//attach the camera to the socket of the arm component
	Camera->AttachTo(Arm, USpringArmComponent::SocketName);

	//default to false because player does not starts jumping
	jumping = false;

	//default first person to false; game starts at 3rd person
	firstPerson = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//check if jump bool is true
	if (jumping)
	{
		//the engine's jump function
		Jump();
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//to bind the axis input component to the movement functions
	InputComponent->BindAxis("Horizontal", this, &APlayerCharacter::HorizontalMovement);
	InputComponent->BindAxis("Vertical", this, &APlayerCharacter::VerticalMovement);

	//to bind the axis input component to the rotation functions
	InputComponent->BindAxis("HorizontalRotation", this, &APlayerCharacter::HorizontalRotation);
	InputComponent->BindAxis("VerticalRotation", this, &APlayerCharacter::VerticalRotation);

	//to bind the axis input for zoom to the zoom function
	InputComponent->BindAxis("Zoom", this, &APlayerCharacter::Zoom);

	//to bind the action input to the check jump function when button pressed, and button released
	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::CheckJump);
	InputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::CheckJump);

	//to bind the action input to the switch function when button pressed
	InputComponent->BindAction("ChangeCamera", IE_Pressed, this, &APlayerCharacter::Switch);

	//to bind shoot action to shoot function when button is pressed
	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::Shoot);
}

//check jump function definition
void APlayerCharacter::CheckJump()
{
	//to change the bool's value to false when PRESSED, and to false when RELEASED (bool will be true as long as button is pressed)
	if (jumping)
	{
		jumping = false;
	}
	else
	{
		jumping = true;
	}
}

//zoom function definition
void APlayerCharacter::Zoom(float value)
{
	//check if value is null
	if (value)
	{
		//create temporary value so that the player does not zooms infinetly
		float temp = Arm->TargetArmLength + (value * -10);

		//check if temp is within zoom limits
		if (temp < 310 && temp > 140)
		{
			Arm->TargetArmLength = temp;
		}
	}
}

//switch function definition
void APlayerCharacter::Switch()
{
	//check perspective
	if (firstPerson)
	{
		//default camera and arm to default 3rd person view attributes
		Arm->TargetArmLength = 300.f;
		Arm->SetRelativeRotation(FRotator(-45, 0.f, 0.f));

		Camera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		Camera->AttachTo(Arm, USpringArmComponent::SocketName);

		//set bool value to false
		firstPerson = false;
	}
	else
	{
		//default camera to first person view
		Camera->AttachTo(RootComponent);

		//set bool value to true
		firstPerson = true;
	}
}

//shoot function definition
void APlayerCharacter::Shoot()
{
	//check if bullet class is not null
	if (BulletClass)
	{
		//creating and setting the bullet spawn parameters for the spawn actor function
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;	//handles collision
		SpawnParams.bNoFail = true;	//it has to spawn; spawn can not fail
		SpawnParams.Owner = this;	//layer character is the owner
		SpawnParams.Instigator = this;	//actor that is responsible for the damage that is dealt by the thing that is spawning

		//creating and setting the bullet transform parameter for the spawn actor function
		FTransform BulletSpawnTransform; 
		BulletSpawnTransform.SetLocation(GetActorForwardVector() * 500.f + GetActorLocation());	//spawn the bullet in front of the player
		BulletSpawnTransform.SetRotation(GetActorRotation().Quaternion());	//whatever rotation the player has, give it to the bullet
		BulletSpawnTransform.SetScale3D(FVector(1.f));	//set the scale of the bullet

		//get the bullet actor; shooting is basically just spawning the actor with projectile movement
		GetWorld()->SpawnActor<ABullet>(BulletClass, BulletSpawnTransform, SpawnParams);
	}
}

//vertical rotation function definition
void APlayerCharacter::VerticalRotation(float value)
{
	//check if value is not null
	if (value)
	{
		//create value for the camera so that the player does not disorients themselves
		float temp = 0;

		//check if perspective is first person
		if (firstPerson)
		{
			//get the relative rotation's pitch and add value
			temp = Camera->GetRelativeRotation().Pitch + value;

			//check if the temp value is within the camera limits
			if (temp < 65 && temp > -65)
			{
				//add the camera rotation
				Camera->AddLocalRotation(FRotator(value, 0.f, 0.f));
			}
		}
		else
		{
			//get the relative rotation's pitch and add value
			temp = Arm->GetRelativeRotation().Pitch + value;

			//check if the temp value is within the camera limits
			if (temp < 25 && temp > -65)
			{
				//add the camera rotation
				Arm->AddLocalRotation(FRotator(value, 0.f, 0.f));
			}
		}
	}
}

//horizontal rotation function definition
void APlayerCharacter::HorizontalRotation(float value)
{
	//to check if value is null
	if (value)
	{
		//adding input rotation
		AddActorLocalRotation(FRotator(0, value, 0));
	}
}

//horizontal movement function definition
void APlayerCharacter::HorizontalMovement(float value)
{
	//to check if value is not null
	if (value)
	{
		//adding the input movement to the character
		AddMovementInput(GetActorRightVector(), value);
	}
}

//vertical movement function definition
void APlayerCharacter::VerticalMovement(float value)
{
	//to check if value is not null
	if (value)
	{
		//adding the input movement to the character
		AddMovementInput(GetActorForwardVector(), value);
	}
}

