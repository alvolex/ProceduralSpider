// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpoopderTestCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProceduralWalk/T4ProceduralLeg.h"

//////////////////////////////////////////////////////////////////////////
// ASpoopderTestCharacter

ASpoopderTestCharacter::ASpoopderTestCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	CubeBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeBody"));
	CubeBody->SetupAttachment(RootComponent);

	//Cursed leg shit
	LegsParent = CreateDefaultSubobject<USceneComponent>(TEXT("LegsParent"));
	LegsParent->SetupAttachment(RootComponent);

	FrontLeftLeg = CreateDefaultSubobject<UChildActorComponent>(TEXT("FrontLeftLeg"));
	FrontLeftLeg->SetupAttachment(LegsParent);

	FrontRightLeg = CreateDefaultSubobject<UChildActorComponent>(TEXT("FrontRightLeg"));
	FrontRightLeg->SetupAttachment(LegsParent);

	BackLeftLeg = CreateDefaultSubobject<UChildActorComponent>(TEXT("BackLeftLEg"));
	BackLeftLeg->SetupAttachment(LegsParent);

	BackRightLeg = CreateDefaultSubobject<UChildActorComponent>(TEXT("BackRightLeg"));
	BackRightLeg->SetupAttachment(LegsParent);

	LegPositions = CreateDefaultSubobject<USceneComponent>(TEXT("LegPositions"));
	LegPositions->SetupAttachment(LegsParent);	

	//Should make a new component that 	
	/*LegPosComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LegComponentPosition"));
	LegPosComponent->SetupAttachment(RootComponent);

	LegTargetComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LegTargetPosition"));
	LegTargetComponent->SetupAttachment(CubeBody);*/
}

void ASpoopderTestCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupSpiderLegs();

	//todo create a component that takes these positions and constucts legs for each position
	TArray<USceneComponent*> ChildComponents;
	LegPositions->GetChildrenComponents(true, ChildComponents);
}

void ASpoopderTestCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASpoopderTestCharacter::SetupSpiderLegs()
{
	//Cuuuursed
	//Todo all the spider stuff needs to be in its own component. Maybe make it more procedural as well so you can have X amount of legs..
	FrontLeftLegCast = Cast<AT4ProceduralLeg>(FrontLeftLeg->GetChildActor());
	FrontRightLegCast = Cast<AT4ProceduralLeg>(FrontRightLeg->GetChildActor());
	BackLeftLegCast = Cast<AT4ProceduralLeg>(BackLeftLeg->GetChildActor());
	BackRightLegCast = Cast<AT4ProceduralLeg>(BackRightLeg->GetChildActor());

	if (FrontLeftLegCast && FrontRightLegCast && BackLeftLegCast && BackRightLegCast)
	{
		FrontLeftLegCast->OppositeLeg = FrontRightLegCast;
		FrontRightLegCast->OppositeLeg = FrontLeftLegCast;
		BackLeftLegCast->OppositeLeg = BackRightLegCast;
		BackRightLegCast->OppositeLeg = BackLeftLegCast;

		FrontLeftLegCast->bIsGrounded = true;
		BackRightLegCast->bIsGrounded = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASpoopderTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ASpoopderTestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ASpoopderTestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ASpoopderTestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ASpoopderTestCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASpoopderTestCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASpoopderTestCharacter::TouchStopped);
}

void ASpoopderTestCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ASpoopderTestCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ASpoopderTestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ASpoopderTestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ASpoopderTestCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASpoopderTestCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
