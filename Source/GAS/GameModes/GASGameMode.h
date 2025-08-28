// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CharacterClassInfo.h"
#include "Projectiles/ProjectileInfo.h"
#include "GASGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AGASGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	AGASGameMode();

	UCharacterClassInfo* GetCharacterClassDefaultInfo() const { return CharacterClassDefaultInfo; }
	UProjectileInfo* GetProjectileInfo() const { return ProjectileInfo; }


private:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCharacterClassInfo> CharacterClassDefaultInfo;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UProjectileInfo> ProjectileInfo;
};



