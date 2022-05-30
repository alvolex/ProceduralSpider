#include "T4ProceduralLegPair.h"

#include "T4ProceduralLeg.h"
#include "SpoopderTest/AnimInstance/SpoderLegAnimInstance.h"

AT4ProceduralLegPair::AT4ProceduralLegPair()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	//Left leg
	LeftLeg = CreateDefaultSubobject<AT4ProceduralLeg>(TEXT("LeftLeg"));
	
	//Right leg
	RightLeg = CreateDefaultSubobject<AT4ProceduralLeg>(TEXT("RightLeg"));
}

void AT4ProceduralLegPair::BeginPlay()
{
	Super::BeginPlay();	
}

void AT4ProceduralLegPair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

}

