// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "GameFramework/Actor.h"
#include "UObject/NoExportTypes.h"

class FToolBarBuilder;
class FMenuBuilder;
struct FCinematicStylePreset;

class FCinematicModeButtonModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	void PluginButton02Clicked();

	
private:
	void RegisterMenus();
	void ApplyCinematicStyle(UWorld* World, const FCinematicStylePreset& Preset);
	

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};

