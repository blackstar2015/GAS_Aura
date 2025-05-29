// Copyright by Pradeep Pillai

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};
/**
 * 
 */
UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()
public:    
    UPROPERTY()
    int32 SlotIndex = 0;

	UPROPERTY()
	int PlayerLevel;
	
    UPROPERTY()
    FString SlotName = FString();
	
	UPROPERTY()
    FString PlayerName = FString("Default Name");

	UPROPERTY()
	FString MapName = FString("Default Map Name");
	
	UPROPERTY()
	FString MapAssetName = FString("Default Map");

	UPROPERTY()
	FName PlayerStartTag;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;
};
