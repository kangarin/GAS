// Fill out your copyright notice in the Description page of Project Settings.


#include "GASSystemsWidget.h"

void UGASSystemsWidget::SetWidgetController(UWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
