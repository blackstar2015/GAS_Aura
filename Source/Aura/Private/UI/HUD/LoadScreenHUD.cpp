// Copyright by Pradeep Pillai


#include "UI/HUD/LoadScreenHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/LoadScreenWidget.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	AActor* LoadScreenBP = GetWorld()->SpawnActor(LoadScreenBlueprint);
	LoadScreenBP->SetActorLocation(LoadScreenSpawnPosition);
}
