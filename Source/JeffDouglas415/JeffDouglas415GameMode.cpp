// Copyright Epic Games, Inc. All Rights Reserved.

#include "JeffDouglas415GameMode.h"
#include "JeffDouglas415Character.h"
#include "UObject/ConstructorHelpers.h"

AJeffDouglas415GameMode::AJeffDouglas415GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
