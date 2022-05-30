#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T4ProceduralLeg.generated.h"

UCLASS()
class SPOOPDERTEST_API AT4ProceduralLeg : public AActor
{
	GENERATED_BODY()
	
public:	
	AT4ProceduralLeg();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* LegSkeletalMesh;

	UPROPERTY(EditAnywhere)
	USceneComponent* LegPosComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* LegTargetComponent;

	UPROPERTY(EditAnywhere)
	float DistanceBeforeTakingNextStep = 180.f;

};
