// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GASGameplayAbility.h"
#include "AbilitySystem/GASAbilityTypes.h"
#include "ProjectileAbility.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UProjectileAbility : public UGASGameplayAbility
{
	GENERATED_BODY()

public:

	UProjectileAbility();

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ProjectileToSpawnTag;

private:

	UPROPERTY()
	TObjectPtr<AActor> AvatarActorFromInfo;
	FProjectileParams CurrentProjectileParams;

	void SpawnProjectile();
	
};
