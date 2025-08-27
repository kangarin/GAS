// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GASSystemsWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UGASSystemsWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	
	void SetWidgetController(class UWidgetController* InWidgetController);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetController> WidgetController;
};
