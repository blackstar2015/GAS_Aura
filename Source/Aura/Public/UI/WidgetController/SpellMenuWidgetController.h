// Copyright by Pradeep Pillai

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadCastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	// UFUNCTION(BlueprintCallable)
	// FGameplayTag GetAbilityStatus();
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnPlayerStatChangeSignature SpellPointsChangeDelegate;

};
