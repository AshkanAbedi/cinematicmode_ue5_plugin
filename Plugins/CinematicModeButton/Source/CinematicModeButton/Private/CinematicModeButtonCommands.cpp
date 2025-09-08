// Copyright Epic Games, Inc. All Rights Reserved.

#include "CinematicModeButtonCommands.h"

#define LOCTEXT_NAMESPACE "FCinematicModeButtonModule"

void FCinematicModeButtonCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "CinematicModeButton", "Execute CinematicModeButton action", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(PluginAction02, "60CinemaStyleButton", "Execute 60CinemaStyleButton action",  EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
