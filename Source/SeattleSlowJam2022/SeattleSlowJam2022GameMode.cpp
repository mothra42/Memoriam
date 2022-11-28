// Copyright Epic Games, Inc. All Rights Reserved.

#include "SeattleSlowJam2022GameMode.h"
#include "SeattleSlowJam2022Character.h"
#include "UObject/ConstructorHelpers.h"

ASeattleSlowJam2022GameMode::ASeattleSlowJam2022GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
