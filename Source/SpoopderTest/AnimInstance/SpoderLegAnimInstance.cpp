#include "SpoderLegAnimInstance.h"

#include "SpoopderTest/SpoopderTestCharacter.h"
#include "SpoopderTest/ProceduralWalk/T4ProceduralLeg.h"

void USpoderLegAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	LegComponent = Cast<AT4ProceduralLeg>(GetOwningActor());
	
	//Get leg start position
	if (LegComponent == nullptr || LegComponent->LegPosComponent == nullptr) return;	
	LegPosition = LegComponent->LegPosComponent->GetComponentLocation();

	//Get leg target position
	if (LegComponent->LegTargetComponent == nullptr) {return;}
	TargetPosition = LegComponent->LegTargetComponent;
}

void USpoderLegAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);	

	//Early Returns
	if (LegComponent == nullptr || TargetPosition == nullptr)
	{
		LegComponent = Cast<AT4ProceduralLeg>(GetOwningActor());
		if (LegComponent != nullptr && LegComponent->LegTargetComponent == nullptr)
		{
			TargetPosition = LegComponent->LegTargetComponent;
		}			
		return;
	}
	if (LegComponent->OppositeLeg && !LegComponent->OppositeLeg->bIsGrounded ) {return;}	

	//Start location for our raycast
	auto StartLoc = TargetPosition->GetComponentLocation();

	//Add some offsets to the front legs || Offset needs to be off when we move the leg on a wall
	if (LegComponent->bIsFrontLeg && !bLegOnWall)
	{
		StartLoc += LegComponent->GetActorForwardVector() * LegComponent->FrontLegOffsetToBody; //Close / Further away from body
		if (LegComponent->GetParentActor())
		{
			StartLoc += LegComponent->GetParentActor()->GetActorForwardVector() * LegComponent->FrontLegOffsetForward;
		}		
	}
	
	//todo Add an overlap sphere to each leg, only raycast if the legs are overlapping with a wall or something idk fam
	//This dirty code is for checking if we need to move the leg up a wall.. This is a lot more cursed than my brain told me 10sec ago
	FHitResult OutHit;	
	if (LegComponent && LegComponent->GetParentActor())
	{
		auto BodyPos = LegComponent->GetParentActor()->GetActorLocation();
		auto LegForward = LegComponent->GetActorForwardVector();
		auto LineEnd = BodyPos + LegForward * 500.f;

		//Cuuuursed offset
		if (LegComponent->bIsFrontLeg)
		{
			LineEnd = BodyPos + LegComponent->GetParentActor()->GetActorForwardVector() * 250.f + LegForward * 500.f;
		}

		//Raycast to wall
		if (GetWorld()->LineTraceSingleByChannel(OutHit, BodyPos, LineEnd, ECollisionChannel::ECC_Visibility))
		{
			LastValidPosition = OutHit.ImpactPoint;
			DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, 6.f, 6, FColor::Cyan);	

			//Check if we're close enough to the wall
			if (FVector::Distance(BodyPos, OutHit.ImpactPoint) < 200.f)
			{
				bLegOnWall = true;
			}
			else
			{
				bLegOnWall = false;
			}

			//Move leg on the wall if we have moved X units from the last step
			if (bLegOnWall && FVector::Distance(LegPosition, OutHit.ImpactPoint) > 150.f)
			{
				MoveToPosition = OutHit.ImpactPoint;
				bIsLerpingPosition = true;
			}	
		}
		else
		{
			bLegOnWall = false;
		}
	}
	
	//Trace down from TargetPosition to find a point we can move towards	
	if (/*!bLegOnWall && */GetWorld()->LineTraceSingleByChannel(OutHit, StartLoc, StartLoc + TargetPosition->GetUpVector() * -350.f, ECollisionChannel::ECC_Visibility))
	{
		DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, 6.f, 6, FColor::Cyan);
		LastValidPosition = OutHit.ImpactPoint;

		if (bLegOnWall && MoveToPosition.Z > OutHit.ImpactPoint.Z)
		{

		}		
		else if (FVector::Distance(OutHit.ImpactPoint, LegPosition) > LegComponent->DistanceBeforeTakingNextStep)
		{
			MoveToPosition = OutHit.ImpactPoint;
			bIsLerpingPosition = true;
		}
	}
	//If we don't get a location from our line trace then we use the last valid location instead
	else if (!bLegOnWall &&FVector::Distance(LastValidPosition, TargetPosition->GetComponentLocation()) > LegComponent->DistanceBeforeTakingNextStep)
	{
		MoveToPosition = LastValidPosition;
		DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, 6.f, 6, FColor::Cyan);
		bIsLerpingPosition = true;
	}
	
	//Lerp leg towards position, the animation blueprint then uses LegPosition to drive the IK
	if (bIsLerpingPosition)
	{
		bIsGrounded = false;
		LegComponent->bIsGrounded = false;
		
		LegPosition = FMath::VInterpConstantTo(LegPosition, MoveToPosition, DeltaSeconds, LegComponent->LegMovementInterpSpeed);
		//LegPosition = FMath::VInterpTo(LegPosition, OutHit.ImpactPoint, DeltaSeconds, 30.f);

		/*LerpProgress += DeltaSeconds * 3.f;
		auto TempPos = FMath::Lerp(LegPosition, MoveToPosition, LerpProgress);

		if (LegComponent->LegZCurve)
		{
			auto LegZ = LegComponent->LegZCurve->GetFloatValue(LerpProgress);
			LegPosition = TempPos + FVector(0,0,LegZ);
		}	*/	

		if (FVector::Distance(MoveToPosition, LegPosition) < LegComponent->CutoffDistanceBeforeBeingGrounded)
		{
			LerpProgress = 0;
			bIsLerpingPosition = false; //Movement completed
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USpoderLegAnimInstance::SetIsGrounded, LegComponent->MinGroundedTime);
		}		
	}
}

void USpoderLegAnimInstance::SetIsGrounded()
{
	bIsGrounded = true;
	if (LegComponent)
	{
		LegComponent->bIsGrounded = true;
	}	
}