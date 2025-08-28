// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CharacterClassInfo.h"
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

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCharacterClassInfo>CharacterClassDefaultInfo;
};



