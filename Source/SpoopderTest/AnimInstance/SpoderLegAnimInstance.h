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
	class AT4ProceduralLegPair* LegPairComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector LegPosition;

	UPROPERTY()
	USceneComponent* TargetPosition;

	UPROPERTY()
	bool bIsLerpingPosition = false;

	UPROPERTY()
	FVector StartLerpPosition;

	UPROPERTY()
	FVector LastValidPosition;

	/*
	 * For zig-zag
	 */	
	UPROPERTY()
	bool bIsRightLeg = false;

	UPROPERTY()
	bool bIsGrounded = false;

public:
	void SetIsRightLeg();
};
