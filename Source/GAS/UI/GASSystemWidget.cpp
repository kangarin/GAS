// Fill out your copyright notice in the Description page of Project Settings.


#include "GASSystemWidget.h"

void UGASSystemWidget::SetWidgetController(UWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
