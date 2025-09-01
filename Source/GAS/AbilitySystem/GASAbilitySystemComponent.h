// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GASAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttributesGiven);
/**
 * 
 */
UCLASS()
class GAS_API UGASAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	FOnAttributesGiven OnAttributesGiven;

	void AddCharacterAbilities(const TArray<TSubclassOf<class UGameplayAbility>>& AbilitiesToGrant);
	void AddCharacterPassives(const TArray<TSubclassOf<class UGameplayAbility>>& PassivesToGrant);
	void InitializeDefaultAttributes(const TSubclassOf<class UGameplayEffect> AttributeEffect);

	void AbilityInputPressed(const FGameplayTag InputTag);
	void AbilityInputReleased(const FGameplayTag InputTag);

	void SetDynamicProjectile(const FGameplayTag& ProjectileTag);

private:
	FGameplayAbilitySpecHandle ActiveProjectileAbility;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> DynamicProjectileAbility;

	UFUNCTION(Server, Reliable)
	void ServerSetDynamicProjectile(const FGameplayTag& ProjectileTag);
};
