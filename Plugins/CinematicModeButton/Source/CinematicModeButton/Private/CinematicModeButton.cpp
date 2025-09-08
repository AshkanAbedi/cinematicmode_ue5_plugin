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
#include "Engine/PostProcessVolume.h"

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

	PluginCommands->MapAction(
		FCinematicModeButtonCommands::Get().PluginAction02,
		FExecuteAction::CreateRaw(this, &FCinematicModeButtonModule::PluginButton02Clicked),
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

	if (AActor* FoundLightActor = FindActorInLevel(ADirectionalLight::StaticClass()))
	{
		ADirectionalLight* LightActor = Cast<ADirectionalLight>(FoundLightActor);
		LightActor->SetActorRotation(FRotator(315.f, -165.f, 221.f));
		LightActor->GetLightComponent()->SetIntensity(1);
		LightActor->GetLightComponent()->SetLightColor(FLinearColor(255.f, 193.f, 212.f));
		LightActor->GetLightComponent()->SetTemperature(4500.f);
	} else
	{
		AActor* NewLightActor = AddActorToLevel(ADirectionalLight::StaticClass());
		ADirectionalLight* LightActor = Cast<ADirectionalLight>(NewLightActor);
		LightActor->SetActorRotation(FRotator(315.f, -165.f, 221.f));
		LightActor->GetLightComponent()->SetIntensity(1);
		LightActor->GetLightComponent()->SetLightColor(FLinearColor(255.f, 193.f, 212.f));
		LightActor->GetLightComponent()->SetTemperature(4500.f);
	}

	if (AActor* FoundPPVolActor = FindActorInLevel(APostProcessVolume::StaticClass()))
	{
		APostProcessVolume* PPVolActor = Cast<APostProcessVolume>(FoundPPVolActor);
		PPVolActor->bUnbound = true;
		PPVolActor->Settings.bOverride_AutoExposureBias = true;
		PPVolActor->Settings.AutoExposureBias = 0;
		PPVolActor->Settings.bOverride_VignetteIntensity = true;
		PPVolActor->Settings.VignetteIntensity = 1.f;
		PPVolActor->Settings.bOverride_FilmGrainIntensity = true;
		PPVolActor->Settings.FilmGrainIntensity = 1.f;
	} else
	{
		AActor* NewPPVolActor = AddActorToLevel(APostProcessVolume::StaticClass());
		APostProcessVolume* PPVolActor = Cast<APostProcessVolume>(NewPPVolActor);
		PPVolActor->bUnbound = true;
		PPVolActor->Settings.bOverride_AutoExposureBias = true;
		PPVolActor->Settings.AutoExposureBias = 0;
		PPVolActor->Settings.bOverride_VignetteIntensity = true;
		PPVolActor->Settings.VignetteIntensity = 1.f;
		PPVolActor->Settings.bOverride_FilmGrainIntensity = true;
		PPVolActor->Settings.FilmGrainIntensity = 1.f;
	}
}

void FCinematicModeButtonModule::PluginButton02Clicked()
{
	FText DialogText = FText::FromString("Enabling 60's Cinematic Style for This Level");
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);

	if (AActor* FoundLightActor = FindActorInLevel(ADirectionalLight::StaticClass()))
	{
		ADirectionalLight* LightActor = Cast<ADirectionalLight>(FoundLightActor);
		LightActor->SetActorRotation(FRotator(315.f, -165.f, 221.f));
		LightActor->GetLightComponent()->SetIntensity(1);
		LightActor->GetLightComponent()->SetLightColor(FLinearColor(255.f, 193.f, 212.f));
		LightActor->GetLightComponent()->SetTemperature(4500.f);
	} else
	{
		AActor* NewLightActor = AddActorToLevel(ADirectionalLight::StaticClass());
		ADirectionalLight* LightActor = Cast<ADirectionalLight>(NewLightActor);
		LightActor->SetActorRotation(FRotator(315.f, -165.f, 221.f));
		LightActor->GetLightComponent()->SetIntensity(1);
		LightActor->GetLightComponent()->SetLightColor(FLinearColor(255.f, 193.f, 212.f));
		LightActor->GetLightComponent()->SetTemperature(4500.f);
	}

	if (AActor* FoundPPVolActor = FindActorInLevel(APostProcessVolume::StaticClass()))
	{
		APostProcessVolume* PPVolActor = Cast<APostProcessVolume>(FoundPPVolActor);
		PPVolActor->bUnbound = true;
		PPVolActor->Settings.bOverride_AutoExposureBias = true;
		PPVolActor->Settings.AutoExposureBias = 0;
		PPVolActor->Settings.bOverride_VignetteIntensity = true;
		PPVolActor->Settings.VignetteIntensity = 1.3f;
		PPVolActor->Settings.bOverride_FilmGrainIntensity = true;
		PPVolActor->Settings.FilmGrainIntensity = 0.8f;
		PPVolActor->Settings.bOverride_ColorSaturation = true;
		PPVolActor->Settings.ColorSaturation = FVector4(1, 1, 1, 0);
	} else
	{
		AActor* NewPPVolActor = AddActorToLevel(APostProcessVolume::StaticClass());
		APostProcessVolume* PPVolActor = Cast<APostProcessVolume>(NewPPVolActor);
		PPVolActor->bUnbound = true;
		PPVolActor->Settings.bOverride_AutoExposureBias = true;
		PPVolActor->Settings.AutoExposureBias = 0;
		PPVolActor->Settings.bOverride_VignetteIntensity = true;
		PPVolActor->Settings.VignetteIntensity = 1.3f;
		PPVolActor->Settings.bOverride_FilmGrainIntensity = true;
		PPVolActor->Settings.FilmGrainIntensity = 0.8f;
		PPVolActor->Settings.bOverride_ColorSaturation = true;
		PPVolActor->Settings.ColorSaturation = FVector4(1, 1, 1, 0);
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

				FToolMenuEntry& Entry02 = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCinematicModeButtonCommands::Get().PluginAction02));
				Entry02.SetCommandList(PluginCommands);
			}
		}
	}
}

AActor* FCinematicModeButtonModule::FindActorInLevel(TSubclassOf<AActor> ActorClass)
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

AActor* FCinematicModeButtonModule::AddActorToLevel(TSubclassOf<AActor> ActorClass)
{
	ULevel* CurrentLevel = GWorld->GetCurrentLevel();
	return CurrentLevel->OwningWorld->SpawnActor<AActor>(ActorClass, FTransform::Identity);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCinematicModeButtonModule, CinematicModeButton)