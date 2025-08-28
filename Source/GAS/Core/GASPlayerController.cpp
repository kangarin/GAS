// Copyright Epic Games, Inc. All Rights Reserved.


#include "GASPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "GAS.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "Inventory/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Input/GASEnhancedInputComponent.h"
#include "Core/GASPlayerState.h"

AGASPlayerController::AGASPlayerController()
{
	bReplicates = true;
	// create inventory component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->SetIsReplicated(true);
}

void AGASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//// only spawn touch controls on local player controllers
	//if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	//{
	//	// spawn the mobile controls widget
	//	MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

	//	if (MobileControlsWidget)
	//	{
	//		// add the controls to the player screen
	//		MobileControlsWidget->AddToPlayerScreen(0);

	//	} else {

	//		UE_LOG(LogGAS, Error, TEXT("Could not spawn mobile controls widget."));

	//	}
	//}
	if (const AGASPlayerState* GASPlayerState = GetPlayerState<AGASPlayerState>()) {
		GASAbilitySystemComponent = GASPlayerState->GetGASAbilitySystemComponent();
	}
}

//void AGASPlayerController::SetupInputComponent()
//{
//	Super::SetupInputComponent();
//
//	// only add IMCs for local player controllers
//	if (IsLocalPlayerController())
//	{
//		// Add Input Mapping Contexts
//		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
//		{
//			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
//			{
//				Subsystem->AddMappingContext(CurrentContext, 0);
//			}
//
//			// only add these IMCs if we're not using mobile touch input
//			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
//			{
//				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
//				{
//					Subsystem->AddMappingContext(CurrentContext, 0);
//				}
//			}
//		}
//	}
//}

void AGASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if(UGASEnhancedInputComponent* CastGASInputComponent = Cast<UGASEnhancedInputComponent>(InputComponent))
	{
		CastGASInputComponent->BindAbilityActions(RPGInputConfig, this, &AGASPlayerController::AbilityInputPressed, &AGASPlayerController::AbilityInputReleased);
	}
}

void AGASPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	if(IsValid(GASAbilitySystemComponent))
	{
		GASAbilitySystemComponent->AbilityInputPressed(InputTag);
	}
}

void AGASPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	if(IsValid(GASAbilitySystemComponent))
	{
		GASAbilitySystemComponent->AbilityInputReleased(InputTag);
	}
}

void AGASPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGASPlayerController, InventoryComponent);
}

UInventoryWidgetController* AGASPlayerController::GetInventoryWidgetController()
{
	if (!IsValid(InventoryWidgetController)) {
		InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetOwningActor(this);
		InventoryWidgetController->BindCallbacksToDependencies();
	}
	return InventoryWidgetController;
}

void AGASPlayerController::CreateInventoryWidget()
{
	if(UUserWidget* NewWidget = CreateWidget<UGASSystemWidget>(this, InventoryWidgetClass) )
	{
		InventoryWidget = Cast<UGASSystemWidget>(NewWidget);
		InventoryWidget->SetWidgetController(GetInventoryWidgetController());
		InventoryWidgetController->BroadcastInitialValues();
		InventoryWidget->AddToViewport();
	}
}
