#include "T4ProceduralLegPair.h"

#include "SpoopderTest/AnimInstance/SpoderLegAnimInstance.h"

AT4ProceduralLegPair::AT4ProceduralLegPair()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	//Left leg
	LeftLeg = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftLeg"));
	LeftLeg->SetupAttachment(RootComponent);

	//Might need to be on the root component(?)
	LeftLegPos = CreateDefaultSubobject<USceneComponent>(TEXT("LeftLegPos"));
	LeftLegPos->SetupAttachment(LeftLeg);

	LeftLegTarget = CreateDefaultSubobject<USceneComponent>(TEXT("LeftLegTarget"));
	LeftLegTarget->SetupAttachment(LeftLeg);	

	//Right leg
	RightLeg = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightLeg"));
	RightLeg->SetupAttachment(RootComponent);

	//Might need to be on the root component(?)
	RightLegPos = CreateDefaultSubobject<USceneComponent>(TEXT("RightLegPos"));
	RightLegPos->SetupAttachment(RightLeg);

	RightLegTarget = CreateDefaultSubobject<USceneComponent>(TEXT("RightLegTarget"));
	RightLegTarget->SetupAttachment(RightLeg);	
}

void AT4ProceduralLegPair::BeginPlay()
{
	Super::BeginPlay();

	if (RightLeg && RightLeg->GetAnimInstance())
	{
		auto RightLegAnimInstance = Cast<USpoderLegAnimInstance>(RightLeg->GetAnimInstance());

		if (RightLegAnimInstance)
		{
			RightLegAnimInstance->SetIsRightLeg();
		}
	}
	
}

void AT4ProceduralLegPair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

}

