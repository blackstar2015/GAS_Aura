// Copyright by Pradeep Pillai

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilityInfo;
class UCharacterClassInfo;

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category="Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag = FName("PlayerStart");

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;
	
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	void TravelToMap(UMVVM_LoadSlot* LoadSlot);
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);
	void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString("")) const;
	void LoadWorldState(UWorld* World) const;
	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;
	ULoadScreenSaveGame* RetrieveInGameSaveData();	
	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void PlayerDied(ACharacter*  DeadCharacter);
protected:
	virtual void BeginPlay() override;
};
