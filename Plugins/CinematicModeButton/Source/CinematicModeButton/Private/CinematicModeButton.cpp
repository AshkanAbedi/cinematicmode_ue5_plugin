// Copyright Epic Games, Inc. All Rights Reserved.

#include "CinematicModeButton.h"
#include "CinematicModeButtonStyle.h"
#include "CinematicModeButtonCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Kismet/GameplayStatics.h"
#include "Editor/EditorEngine.h"
#include "Engine/Level.h"
#include "Engine/DirectionalLight.h"
#include "Components/LightComponent.h"

static const FName CinematicModeButtonTabName("CinematicModeButton");

#define LOCTEXT_NAMESPACE "FCinematicModeButtonModule"

void FCinematicModeButtonModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FCinematicModeButtonStyle::Initialize();
	FCinematicModeButtonStyle::ReloadTextures();

	FCinematicModeButtonCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FCinematicModeButtonCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FCinematicModeButtonModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCinematicModeButtonModule::RegisterMenus));
}

void FCinematicModeButtonModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FCinematicModeButtonStyle::Shutdown();

	FCinematicModeButtonCommands::Unregister();
}

void FCinematicModeButtonModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::FromString("Enabling Cinematic Mode for This Level");
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);

	if (AActor* FoundLightActor = FindActor(ADirectionalLight::StaticClass()))
	{
		ADirectionalLight* LightActor = Cast<ADirectionalLight>(FoundLightActor);
		LightActor->GetLightComponent()->SetIntensity(1);
	} else
	{
		AActor* NewLightActor = AddActorToScene(ADirectionalLight::StaticClass());
		ADirectionalLight* LightActor = Cast<ADirectionalLight>(NewLightActor);
		LightActor->GetLightComponent()->SetIntensity(1);
	}
}

void FCinematicModeButtonModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FCinematicModeButtonCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCinematicModeButtonCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

AActor* FCinematicModeButtonModule::FindActor(TSubclassOf<AActor> ActorClass)
{
	TArray<AActor*> FoundActors;

	const UObject* WorldContextObject = GWorld;

	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, ActorClass, FoundActors);

	if (FoundActors.Num() > 0)
	{
		return FoundActors[0];
	}
		
	return nullptr;
}

AActor* FCinematicModeButtonModule::AddActorToScene(TSubclassOf<AActor> ActorClass)
{
	ULevel* CurrentLevel = GWorld->GetCurrentLevel();
	return CurrentLevel->OwningWorld->SpawnActor<AActor>(ActorClass, FTransform::Identity);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCinematicModeButtonModule, CinematicModeButton)