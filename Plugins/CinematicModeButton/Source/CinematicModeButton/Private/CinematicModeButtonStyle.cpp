// Copyright Epic Games, Inc. All Rights Reserved.

#include "CinematicModeButtonStyle.h"
#include "CinematicModeButton.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FCinematicModeButtonStyle::StyleInstance = nullptr;

void FCinematicModeButtonStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FCinematicModeButtonStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FCinematicModeButtonStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("CinematicModeButtonStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FCinematicModeButtonStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("CinematicModeButtonStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("CinematicModeButton")->GetBaseDir() / TEXT("Resources"));

	Style->Set("CinematicModeButton.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FCinematicModeButtonStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FCinematicModeButtonStyle::Get()
{
	return *StyleInstance;
}
