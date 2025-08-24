// Fill out your copyright notice in the Description page of Project Settings.


#include "GASAbilitySystemComponent.h"

void UGASAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<class UGameplayAbility>>& AbilitiesToGrant)
{
	for(const TSubclassOf<UGameplayAbility>& Ability : AbilitiesToGrant)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
		GiveAbility(AbilitySpec);
	}
}

void UGASAbilitySystemComponent::AddCharacterPassives(const TArray<TSubclassOf<class UGameplayAbility>>& PassivesToGrant)
{
	for(const TSubclassOf<UGameplayAbility>& Passive : PassivesToGrant)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Passive, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UGASAbilitySystemComponent::InitializeDefaultAttributes(const TSubclassOf<class UGameplayEffect> AttributeEffect)
{
	checkf(AttributeEffect, TEXT("AttributeEffect is null! Please check the CharacterClassInfo for the character."));
	const FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(AttributeEffect, 1, ContextHandle);
	ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
