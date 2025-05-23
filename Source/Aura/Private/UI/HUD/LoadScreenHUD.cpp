// Copyright by Pradeep Pillai


#include "UI/HUD/LoadScreenHUD.h"

#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
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
	
	//Spawn Loadscreen in WORLD space 
	AActor* LoadScreenBP = GetWorld()->SpawnActor(LoadScreenBlueprint);
	LoadScreenBP->SetActorLocation(LoadScreenSpawnPosition);
	//Find the Loadscreen widget in the LoadscreenBP's components
	if (UWidgetComponent* LoadScreenWidgetComponent = LoadScreenBP->FindComponentByClass<UWidgetComponent>())
	{
		UUserWidget* LoadScreenWidgetObjectUserWidget = LoadScreenWidgetComponent->GetUserWidgetObject();
		LoadScreenWidget = Cast<ULoadScreenWidget>(LoadScreenWidgetObjectUserWidget);
		if (LoadScreenWidget)
		{
			LoadScreenWidget->BlueprintInitializeWidget();			
		}
	}
	//LoadScreenViewModel->LoadData();
}
