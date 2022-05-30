#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T4ProceduralLegPair.generated.h"

UCLASS()
class SPOOPDERTEST_API AT4ProceduralLegPair : public AActor
{
	GENERATED_BODY()
	
public:
	AT4ProceduralLegPair();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	/*Left Leg*/
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* LeftLeg;

	/*Right Leg*/
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* RightLeg;	

};
