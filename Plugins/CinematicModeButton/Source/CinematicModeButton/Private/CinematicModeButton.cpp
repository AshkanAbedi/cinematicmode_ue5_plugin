// Copyright Epic Games, Inc. All Rights Reserved.

#include "CinematicModeButton.h"
#include "CinematicModeButtonStyle.h"
#include "CinematicModeButtonCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

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
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FCinematicModeButtonModule::PluginButtonClicked()")),
							FText::FromString(TEXT("CinematicModeButton.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
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

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCinematicModeButtonModule, CinematicModeButton)