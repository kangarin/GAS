// Fill out your copyright notice in the Description page of Project Settings.


#include "GASPlayerState.h"
#include "AbilitySystem/GASAbilitySystemComponent.h"
#include "AbilitySystem/GASAttributeSet.h"

AGASPlayerState::AGASPlayerState()
{
	NetUpdateFrequency = 100.f;
	MinNetUpdateFrequency = 66.f;

	GASAbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("GASAbilitySystemComponent"));
	GASAbilitySystemComponent->SetIsReplicated(true);
	GASAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	GASAttributeSet = CreateDefaultSubobject<UGASAttributeSet>(TEXT("GASAttributeSet"));
}

UAbilitySystemComponent* AGASPlayerState::GetAbilitySystemComponent() const
{
	return GASAbilitySystemComponent;
}
