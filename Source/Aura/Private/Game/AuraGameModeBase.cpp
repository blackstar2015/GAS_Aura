// Copyright by Pradeep Pillai


#include "Game/AuraGameModeBase.h"

#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	//Remove save slot if there already exists one
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}
	//Create new save game object
	USaveGame* SaveGameObject =  UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	// Cast to Load Screen Save game.
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	// Save player name to the save slot
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	//SAVE GAME
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}
