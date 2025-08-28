// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/GASAbilitySystemComponent.h"
#include "AbilitySystem/GASAttributeSet.h"
#include "EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API AEnemyBase : public ACharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AEnemyBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UGASAbilitySystemComponent> GASAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UGASAttributeSet> GASAttributeSet;

	UPROPERTY(ReplicatedUsing = OnRep_InitAttributes)
	bool bInitAttributes = false;

	UFUNCTION()
	void OnRep_InitAttributes();
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void BindCallbacksToDependencies() override;
	virtual void InitClassDefaults() override;
	virtual void BroadcastInitialValues() override;
};
