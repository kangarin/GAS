// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyBase.h"
#include "AbilitySystem/GASAbilitySystemLibrary.h"
#include "Character/CharacterClassInfo.h"


AEnemyBase::AEnemyBase()
{
	bReplicates = true;
	GASAbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("GASAbilitySystemComponent"));
	GASAbilitySystemComponent->SetIsReplicated(true);
	// because this is ai controlled, we can set to minimal
	GASAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	GASAttributeSet = CreateDefaultSubobject<UGASAttributeSet>(TEXT("GASAttributeSet"));
}

UAbilitySystemComponent* AEnemyBase::GetAbilitySystemComponent() const
{
	return GASAbilitySystemComponent;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		InitAbilityActorInfo();
	}
}

void AEnemyBase::InitAbilityActorInfo()
{
	if(IsValid(GASAbilitySystemComponent) && IsValid(GASAttributeSet))
	{
		GASAbilitySystemComponent->InitAbilityActorInfo(this, this);

		if(HasAuthority()){
			InitClassDefaults();
		}
	}
}

void AEnemyBase::InitClassDefaults()
{
	if(!CharacterTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("'%s' CharacterTag is not set! Please set a CharacterTag for this character class."), *GetNameSafe(this));
	}
	else if(UCharacterClassInfo* ClassInfo = UGASAbilitySystemLibrary::GetCharacterDefaultInfo(this)) 
	{
		if (FCharacterClassDefaultInfo* SelectedClassInfo = ClassInfo->ClassDefaultInfoMap.Find(CharacterTag))
		{
			if (IsValid(GASAbilitySystemComponent))
			{
				GASAbilitySystemComponent->AddCharacterAbilities(SelectedClassInfo->StartingAbilities);
				GASAbilitySystemComponent->AddCharacterPassives(SelectedClassInfo->StartingPassives);
				GASAbilitySystemComponent->InitializeDefaultAttributes(SelectedClassInfo->DefaultAttributes);
			}
		}
	}
}

void AEnemyBase::BindCallbacksToDependencies()
{
	if (IsValid(GASAbilitySystemComponent) && IsValid(GASAttributeSet)) {
		GASAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GASAttributeSet->GetHealthAttribute()).AddLambda(
			[&](const FOnAttributeChangeData& Data)
			{
				const float CurrentHealth = Data.NewValue;
				const float MaxHealth = GASAttributeSet->GetMaxHealth();
				OnHealthChanged(CurrentHealth, MaxHealth);
			}
		);

		GASAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GASAttributeSet->GetManaAttribute()).AddLambda(
			[&](const FOnAttributeChangeData& Data)
			{
				const float CurrentMana = Data.NewValue;
				const float MaxMana = GASAttributeSet->GetMaxMana();
				OnManaChanged(CurrentMana, MaxMana);
			}
		);
	}

}
