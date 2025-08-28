// Fill out your copyright notice in the Description page of Project Settings.


#include "GASAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/GASGameMode.h"

UCharacterClassInfo* UGASAbilitySystemLibrary::GetCharacterDefaultInfo(const UObject* WorldContextObject)
{
	if(AGASGameMode* GASGameMode = Cast<AGASGameMode>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return GASGameMode->GetCharacterClassDefaultInfo();
	}
	return nullptr;
}

UProjectileInfo* UGASAbilitySystemLibrary::GetProjectileInfo(const UObject* WorldContextObject)
{
	if(AGASGameMode* GASGameMode = Cast<AGASGameMode>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return GASGameMode->GetProjectileInfo();
	}
	return nullptr;
}
