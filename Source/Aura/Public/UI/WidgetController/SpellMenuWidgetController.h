// Copyright by Pradeep Pillai

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled,bool,bEquipButtonEnabled);

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};
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

	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Delegates")
	FOnPlayerStatChangeSignature SpellPointsChangeDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|Delegates")
	FSpellGlobeSelectedSignature FSpellGlobeSelectedDelegate;

private:
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int SpellPoints, bool& bShouldEnableSpendPointsButton, bool& bShouldEnableEquipButton );
	
	FSelectedAbility SelectedAbility = {FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked};

	int32 CurrentSpellPoints = 0;
};
