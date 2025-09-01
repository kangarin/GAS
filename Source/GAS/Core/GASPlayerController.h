// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryInterface.h"
#include "Inventory/InventoryWidgetController.h"
#include "UI/GASSystemWidget.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/GASAbilitySystemComponent.h"
#include "Input/GASInputConfig.h"
#include "Input/GASEnhancedInputComponent.h"
#include "AbilitySystem/GASAbilitySystemInterface.h"
#include "GASPlayerController.generated.h"


class UInputMappingContext;
class UUserWidget;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class AGASPlayerController : public APlayerController, public IAbilitySystemInterface, public IInventoryInterface, public IGASAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGASPlayerController();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
	}

	virtual UInventoryComponent* GetInventoryComponent_Implementation() override
	{
		return InventoryComponent;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetDynamicProjectile_Implementation(const FGameplayTag& ProjectileTag) override;

	UInventoryWidgetController* GetInventoryWidgetController();

	UFUNCTION(BlueprintCallable)
	void CreateInventoryWidget();
	
protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);

private:

	UPROPERTY()
	TObjectPtr<UGASAbilitySystemComponent> GASAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UGASEnhancedInputComponent> GASInputComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UGASInputConfig> GASInputConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UGASSystemWidget> InventoryWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGASSystemWidget> InventoryWidgetClass;

	UGASAbilitySystemComponent* GetGASAbilitySystemComponent() const;

};
