// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "CinematicModeButtonStyle.h"

class FCinematicModeButtonCommands : public TCommands<FCinematicModeButtonCommands>
{
public:

	FCinematicModeButtonCommands()
		: TCommands<FCinematicModeButtonCommands>(TEXT("CinematicModeButton"), NSLOCTEXT("Contexts", "CinematicModeButton", "CinematicModeButton Plugin"), NAME_None, FCinematicModeButtonStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
	TSharedPtr< FUICommandInfo > PluginAction02;
};
