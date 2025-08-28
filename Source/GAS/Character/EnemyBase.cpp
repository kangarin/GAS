// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyBase.h"
#include "AbilitySystem/GASAbilitySystemLibrary.h"
#include "Character/CharacterClassInfo.h"
#include <Net/UnrealNetwork.h>


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

void AEnemyBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnemyBase, bInitAttributes);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	BindCallbacksToDependencies();
	InitAbilityActorInfo();

}

void AEnemyBase::InitAbilityActorInfo()
{
	if(IsValid(GASAbilitySystemComponent) && IsValid(GASAttributeSet))
	{
		GASAbilitySystemComponent->InitAbilityActorInfo(this, this);

		if(HasAuthority()){
			InitClassDefaults();
			BroadcastInitialValues();
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

		if (HasAuthority()) {
			GASAbilitySystemComponent->OnAttributesGiven.AddLambda([&]() {
				bInitAttributes = true;
				});
		}
	}
}

void AEnemyBase::BroadcastInitialValues()
{
	if(IsValid(GASAttributeSet))
	{
		OnHealthChanged(GASAttributeSet->GetHealth(), GASAttributeSet->GetMaxHealth());
		OnManaChanged(GASAttributeSet->GetMana(), GASAttributeSet->GetMaxMana());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to broadcast initial values! Attribute Set is invalid."), *GetNameSafe(this));
	}
}

void AEnemyBase::OnRep_InitAttributes()
{
	BroadcastInitialValues();
}