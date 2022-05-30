// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpoopderTestGameMode.h"
#include "SpoopderTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASpoopderTestGameMode::ASpoopderTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
