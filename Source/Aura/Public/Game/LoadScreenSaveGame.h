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

	UPROPERTY()
	bool bFirstTimeLoadIn = true;
	
	//Player Properties
	UPROPERTY()
	int32 PlayerLevel = 1;

	UPROPERTY()
	int32 Xp = 0;

	UPROPERTY()
	int32 SpellPoints = 0;

	UPROPERTY()
	int32 AttributePoints = 0;

	//Primary Attributes
	UPROPERTY()
	float Strength = 0;

	UPROPERTY()
	float Intelligence = 0;
	
	UPROPERTY()
	float Resilience = 0;

	UPROPERTY()
	float Vigour = 0;
};
