#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpoopderTestCharacter.generated.h"

UCLASS(config=Game)
class ASpoopderTestCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ASpoopderTestCharacter();
	virtual void BeginPlay() override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CubeBody;

	UPROPERTY(EditAnywhere)
	USceneComponent* LegPosComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* LegTargetComponent;

	UPROPERTY(EditAnywhere)
	float DistanceBeforeTakingNextStep = 200.f;

	/*
	 * Legs (Should probably move these badbois to a component)
	 */
	UPROPERTY()
	class USceneComponent* LegsParent;
	
	/*UPROPERTY()
	class AT4ProceduralLeg* FrontLeftLeg;

	UPROPERTY(EditAnywhere)
	class AT4ProceduralLeg* FrontRightLeg;

	UPROPERTY(EditAnywhere)
	class AT4ProceduralLeg* BackLeftLeg;

	UPROPERTY(EditAnywhere)
	class AT4ProceduralLeg* BackRightLeg;*/

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* FrontLeftLeg;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* FrontRightLeg;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* BackLeftLeg;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* BackRightLeg;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

