// Copyright by Pradeep Pillai


#include "UI/HUD/LoadScreenHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/Widget/LoadScreenWidget.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	//Creating the loadscreen view model before creating the load screen
	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModeClass);
	//Assign View Model Class to Load Slots
	LoadScreenViewModel->InitializeLoadSlots();

/**
 * Spawn Loadscreen in SCREEN space
 * LoadScreenWidget =CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
 * LoadScreenWidget->AddToViewport();
 */
	
	/**Spawn Loadscreen in WORLD space */
	AActor* LoadScreenBP = GetWorld()->SpawnActor(LoadScreenBlueprint);
	LoadScreenBP->SetActorLocation(LoadScreenSpawnPosition);

	LoadScreenWidget = LoadScreenBP->GetComponentByClass<ULoadScreenWidget>();
	LoadScreenWidget->BlueprintInitializeWidget();
}
