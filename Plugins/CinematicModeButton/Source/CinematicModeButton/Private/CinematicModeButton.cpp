// Copyright Epic Games, Inc. All Rights Reserved.
// ReSharper disable CppMemberFunctionMayBeStatic
// ReSharper disable CppTooWideScope
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

struct FCinematicLightPreset
{
	FRotator Rotation = FRotator::ZeroRotator;
	float Intensity = 1.0f;
	FLinearColor Color = FLinearColor::White;
	float Temperature = 6500.0f;
};

struct FCinematicPostProcessPreset
{
	bool bOverrideExposure = true;
	float ExposureBias = 0.f;

	bool bOverrideVignette = true;
	float VignetteIntensity = 1.f;

	bool bOverrideFilmGrain = true;
	float FilmGrainIntensity = 1.f;

	bool bOverrideSaturation = false;
	FVector4 ColorSaturation = FVector4(1, 1, 1, 0);
};

struct FCinematicStylePreset
{
	FString Name;
	FCinematicLightPreset Light;
	FCinematicPostProcessPreset PostProcess;
};

const FCinematicStylePreset DefaultCinematicPreset{
	"Default Mode",
{FRotator(315.f, -165.f, 221.f), 1.f, FLinearColor(255.f,193.f,212.f), 4500.f},
{true, 0.f, true, 1.f, true, 1.f, false, FVector4(1,1,1,0)}
};

const FCinematicStylePreset SixtiesCinematicPreset {
	"60's Style Mode",
{FRotator(315.f, -165.f, 221.f), 1.f, FLinearColor(255.f,193.f,212.f), 4500.f},
{true, 0.f, true, 1.3f, true, 0.8f, true, FVector4(1,1,1,0)}
};

template <typename T>
T* FindOrAddActor(UWorld* World, TFunction<void(T*)> InitFunc)
{
	if (!World) return nullptr;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, T::StaticClass(), FoundActors);
	T* Actor = nullptr;

	if (FoundActors.Num() > 0)
	{
		Actor = Cast<T>(FoundActors[0]);
	} else
	{
		Actor = World->SpawnActor<T>(T::StaticClass(), FTransform::Identity);
	}

	if (Actor && InitFunc)
	{
		InitFunc(Actor);
	}
	return Actor;
}

void FCinematicModeButtonModule::ApplyCinematicStyle(UWorld* World, const FCinematicStylePreset& Preset)
{
	if (!World) return;

	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Applying " + Preset.Name + "!"));
	
	FindOrAddActor<ADirectionalLight>(World, [&](ADirectionalLight* LightActor)
	{
		ULightComponent* LightComp = LightActor->GetLightComponent();
		LightActor->SetActorRotation(Preset.Light.Rotation);
		LightComp->SetIntensity(Preset.Light.Intensity);
		LightComp->SetLightColor(Preset.Light.Color);
		LightComp->SetTemperature(Preset.Light.Temperature);
	});
	
	FindOrAddActor<APostProcessVolume>(World, [&](APostProcessVolume* PPVolActor)
	{
		PPVolActor->bUnbound = true;

		if (Preset.PostProcess.bOverrideExposure)
		{
			PPVolActor->Settings.bOverride_AutoExposureBias = true;
			PPVolActor->Settings.AutoExposureBias = Preset.PostProcess.ExposureBias;
		}

		if (Preset.PostProcess.bOverrideVignette)
		{
			PPVolActor->Settings.bOverride_VignetteIntensity = true;
			PPVolActor->Settings.VignetteIntensity = Preset.PostProcess.VignetteIntensity;
		}

		if (Preset.PostProcess.bOverrideFilmGrain)
		{
			PPVolActor->Settings.bOverride_FilmGrainIntensity = true;
			PPVolActor->Settings.FilmGrainIntensity = Preset.PostProcess.FilmGrainIntensity;
		}

		if (Preset.PostProcess.bOverrideSaturation)
		{
			PPVolActor->Settings.bOverride_ColorSaturation = true;
			PPVolActor->Settings.ColorSaturation = Preset.PostProcess.ColorSaturation;
		}
	});
	
}

void FCinematicModeButtonModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	ApplyCinematicStyle(GWorld, DefaultCinematicPreset);
}

void FCinematicModeButtonModule::PluginButton02Clicked()
{
	// Put your "OnButtonClicked" stuff here
	ApplyCinematicStyle(GWorld, SixtiesCinematicPreset);
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

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCinematicModeButtonModule, CinematicModeButton)