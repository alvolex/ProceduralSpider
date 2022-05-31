#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SpoderLegAnimInstance.generated.h"


UCLASS()
class SPOOPDERTEST_API USpoderLegAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;	

private:	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ASpoopderTestCharacter* Character;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AT4ProceduralLeg* LegComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector LegPosition;

	UPROPERTY()
	USceneComponent* TargetPosition;

	UPROPERTY()
	bool bIsLerpingPosition = false;

	UPROPERTY()
	FVector MoveToPosition;

	UPROPERTY()
	FVector LastValidPosition;

	/*
	 * For zig-zag
	 */	
	UPROPERTY()
	bool bIsGrounded = false;

	UPROPERTY()
	bool bCanMove = false;

	UPROPERTY()
	bool bLegOnWall = false;

	UPROPERTY()
	float LerpProgress = 0.f;

private:
	void SetIsGrounded();
};
