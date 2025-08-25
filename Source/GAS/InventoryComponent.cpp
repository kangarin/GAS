// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"

bool FPackagedInventory::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	SafeNetSerializeTArray_WithNetSerialize<100>(Ar, ItemTags, Map);
	SafeNetSerializeTArray_Default<100>(Ar, ItemCounts);

	bOutSuccess = (ItemTags.Num() == ItemCounts.Num());
	return true;
}

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true); 

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// DOREPLIFETIME(UInventoryComponent, CachedInventory);
	// 仅复制给拥有此组件的客户端（本地玩家）
    DOREPLIFETIME_CONDITION(UInventoryComponent, CachedInventory, COND_OwnerOnly);
}

void UInventoryComponent::AddItem(const FGameplayTag& ItemTag, int32 Count)
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;

	if (!Owner->HasAuthority())
	{
		ServerAddItem(ItemTag, Count);
		return;
	}
	if (InventoryMap.Contains(ItemTag))
	{
		InventoryMap[ItemTag] += Count;
	}
	else
	{
		InventoryMap.Emplace(ItemTag, Count);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, 
		FString::Printf(TEXT("Added %d of item %s. Total now: %d"), Count, *ItemTag.ToString(), InventoryMap[ItemTag]));

	PackageInventory(CachedInventory);
}

void UInventoryComponent::ServerAddItem_Implementation(const FGameplayTag& ItemTag, int32 Count)
{
	AddItem(ItemTag, Count);
}

void UInventoryComponent::PackageInventory(FPackagedInventory& OutInventory)
{
	OutInventory.ItemTags.Empty();
	OutInventory.ItemCounts.Empty();
	for (const TPair<FGameplayTag, int32>& Pair : InventoryMap)
	{
		if (Pair.Value > 0) {
			OutInventory.ItemTags.Add(Pair.Key);
			OutInventory.ItemCounts.Add(Pair.Value);
		}
	}
}

void UInventoryComponent::ReconstructInventoryMap(const FPackagedInventory& PackagedInventory)
{
	InventoryMap.Empty();
	const int32 NumItems = PackagedInventory.ItemTags.Num();
	for (int32 Index = 0; Index < NumItems; ++Index)
	{
		InventoryMap.Emplace(PackagedInventory.ItemTags[Index], PackagedInventory.ItemCounts[Index]);
	}
}

void UInventoryComponent::OnRep_CachedInventory()
{
	ReconstructInventoryMap(CachedInventory);
}