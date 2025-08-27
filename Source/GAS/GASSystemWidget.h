// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetController.h"
#include "GASSystemWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UGASSystemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetWidgetController(UWidgetController* InWidgetController);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();

private:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetController> WidgetController;
	
};
