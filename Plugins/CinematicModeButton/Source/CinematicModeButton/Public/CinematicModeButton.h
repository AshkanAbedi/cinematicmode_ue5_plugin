// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "GameFramework/Actor.h"
#include "UObject/NoExportTypes.h"

class FToolBarBuilder;
class FMenuBuilder;

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
	static AActor* FindActorInLevel(TSubclassOf<AActor> ActorClass);
	static AActor* AddActorToLevel(TSubclassOf<AActor> ActorClass);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};

