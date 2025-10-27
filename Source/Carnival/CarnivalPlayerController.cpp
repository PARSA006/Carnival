// Copyright Epic Games, Inc. All Rights Reserved.


#include "CarnivalPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "CarnivalCameraManager.h"
#include "Blueprint/UserWidget.h"
#include "Carnival.h"
#include "Widgets/Input/SVirtualJoystick.h"

ACarnivalPlayerController::ACarnivalPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = ACarnivalCameraManager::StaticClass();
}

void ACarnivalPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogCarnival, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void ACarnivalPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
	
}
