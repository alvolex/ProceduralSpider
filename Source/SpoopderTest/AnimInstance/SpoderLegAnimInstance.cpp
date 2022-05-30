#include "SpoderLegAnimInstance.h"

#include "SpoopderTest/SpoopderTestCharacter.h"

void USpoderLegAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Character = Cast<ASpoopderTestCharacter>(TryGetPawnOwner());

	//Get leg start position
	if (Character == nullptr || Character->LegPosComponent == nullptr) return;	
	LegPosition = Character->LegPosComponent->GetComponentLocation();

	//Get leg target position
	if (Character->LegTargetComponent == nullptr) {return;}
	TargetPosition = Character->LegTargetComponent;
}

void USpoderLegAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character == nullptr)
	{
		Character = Cast<ASpoopderTestCharacter>(TryGetPawnOwner());
		if (Character == nullptr || Character->LegTargetComponent != nullptr) {return;}

		TargetPosition = Character->LegTargetComponent;
	}	
	
	if (TargetPosition == nullptr) {return;}

	FHitResult OutHit;
	//Trace down from TargetPosition to find a point we can move towards
	if (GetWorld()->LineTraceSingleByChannel(OutHit, TargetPosition->GetComponentLocation(), TargetPosition->GetComponentLocation() + TargetPosition->GetUpVector() * -350.f, ECollisionChannel::ECC_Visibility))
	{
		DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, 6.f, 6, FColor::Cyan);
		LastValidPosition = OutHit.ImpactPoint;

		if (FVector::Distance(OutHit.ImpactPoint, LegPosition) > Character->DistanceBeforeTakingNextStep)
		{
			StartLerpPosition = LegPosition;
			//LegPosition = OutHit.ImpactPoint;
			bIsLerpingPosition = true;
		}
	}
	//If we don't get a location from our line trace then we use the last valid location instead
	else if (FVector::Distance(LastValidPosition, TargetPosition->GetComponentLocation()) > Character->DistanceBeforeTakingNextStep)
	{
		OutHit.ImpactPoint = LastValidPosition;
		DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, 6.f, 6, FColor::Cyan);
		bIsLerpingPosition = true;
	}

	//Lerp leg towards position, the animation blueprint then uses LegPosition to drive the IK
	if (bIsLerpingPosition)
	{
		LegPosition = FMath::VInterpConstantTo(LegPosition, OutHit.ImpactPoint, DeltaSeconds, 1800.f);
		//LegPosition = FMath::VInterpTo(LegPosition, OutHit.ImpactPoint, DeltaSeconds, 18.f);

		if (FVector::Distance(OutHit.ImpactPoint, LegPosition) < 35.f)
		{
			bIsLerpingPosition = false;

			//Test code
			bIsGrounded = true;
		}		
	}
}

void USpoderLegAnimInstance::SetIsRightLeg()
{
	bIsRightLeg = true;
	bIsGrounded = true;
}
