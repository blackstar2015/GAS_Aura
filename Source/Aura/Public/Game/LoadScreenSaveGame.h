// Copyright by Pradeep Pillai

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

USTRUCT(BlueprintType)
struct FSavedActor
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActorName = FName();

	UPROPERTY()
	FTransform Transform = FTransform();

	//Serialized variables from the actor - only those marked with saveGame specifier
	UPROPERTY()
	TArray<uint8> Bytes;
};

inline bool operator==(const FSavedActor& lhs, const FSavedActor& rhs)
{
	return lhs.ActorName == rhs.ActorName;
}

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityStatus = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilitySlot = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityType = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AbilityLevel;
};

inline bool operator==(const FSavedAbility& lhs, const FSavedAbility& rhs)
{
	return lhs.AbilityTag.MatchesTagExact(rhs.AbilityTag);
}

USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapAssetName = FString();

	UPROPERTY()
	TArray<FSavedActor> SavedActors;

	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;
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

	//Abilities
	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;

	UPROPERTY()
	TArray<FSavedMap> SavedMaps;

	FSavedMap GetSavedMapWithMapName(const FString& InMapName);
	bool HasMap(const FString& InMapName);
};
