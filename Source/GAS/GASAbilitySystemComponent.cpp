// Fill out your copyright notice in the Description page of Project Settings.


#include "GASAbilitySystemComponent.h"
#include "GASGameplayAbility.h"

void UGASAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<class UGameplayAbility>>& AbilitiesToGrant)
{
	for(const TSubclassOf<UGameplayAbility>& Ability : AbilitiesToGrant)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);

		if(const UGASGameplayAbility* GASAbility = Cast<UGASGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(GASAbility->InputTag);
		}
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

void UGASAbilitySystemComponent::AbilityInputPressed(const FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	
	ABILITYLIST_SCOPE_LOCK();

	for(const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if(Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if(!Spec.IsActive())
			{
				TryActivateAbility(Spec.Handle);
			}
			else {
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}

}

void UGASAbilitySystemComponent::AbilityInputReleased(const FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	
	ABILITYLIST_SCOPE_LOCK();
	for(const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if(Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}