#include "T4ProceduralLegPair.h"

#include "SpoopderTest/AnimInstance/SpoderLegAnimInstance.h"

AT4ProceduralLegPair::AT4ProceduralLegPair()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	//Left leg
	LeftLeg = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftLeg"));
	LeftLeg->SetupAttachment(RootComponent);

	//Right leg
	RightLeg = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightLeg"));
	RightLeg->SetupAttachment(RootComponent);
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

