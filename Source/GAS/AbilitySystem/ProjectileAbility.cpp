// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ProjectileAbility.h"
#include "Projectiles/ProjectileInfo.h"
#include "AbilitySystem/GASAbilitySystemLibrary.h"

UProjectileAbility::UProjectileAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}
void UProjectileAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	AvatarActorFromInfo = GetAvatarActorFromActorInfo();

	if(!ProjectileToSpawnTag.IsValid() || !IsValid(AvatarActorFromInfo))
	{
		return;
	}
	if (UProjectileInfo* ProjectileInfo = UGASAbilitySystemLibrary::GetProjectileInfo(GetAvatarActorFromActorInfo())) 
	{
		CurrentProjectileParams = *ProjectileInfo->ProjectileInfoMap.Find(ProjectileToSpawnTag);

	}

	
}

void UProjectileAbility::SpawnProjectile()
{
	if(!IsValid(CurrentProjectileParams.ProjectileClass))
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Spawning Projectile %s"), *CurrentProjectileParams.ProjectileClass->GetName()));
}
