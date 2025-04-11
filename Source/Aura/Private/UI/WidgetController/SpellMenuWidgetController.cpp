// Copyright by Pradeep Pillai


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadCastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChangeDelegate.Broadcast(GetAuraPlayerState()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	
	GetAuraAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
	{
		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status = StatusTag;
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(StatusTag,CurrentSpellPoints,bEnableSpendPoints,bEnableEquip);
			FSpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints,bEnableEquip);			
		}
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
	GetAuraPlayerState()->OnSpellPointChangedDelegate.AddLambda(
		[this](int32 SpellPoints)
		{
			SpellPointsChangeDelegate.Broadcast(SpellPoints);
			CurrentSpellPoints = SpellPoints;
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(SelectedAbility.Status,CurrentSpellPoints,bEnableSpendPoints,bEnableEquip);
			FSpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints,bEnableEquip);
		}
	);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	FGameplayTag AbilityStatus;
	
	const int32 SpellPoints = GetAuraPlayerState()->GetSpellPoints();
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const FGameplayAbilitySpec* AbilitySpec =  GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag);

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const bool bSpecValid = AbilitySpec != nullptr;
	
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraAbilitySystemComponent()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;

	ShouldEnableButtons(AbilityStatus,SpellPoints,bEnableSpendPoints,bEnableEquip);
	FSpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints,bEnableEquip);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAuraAbilitySystemComponent())
		GetAuraAbilitySystemComponent()->ServerSpendSpellPoint(SelectedAbility.Ability);
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int SpellPoints,
                                                     bool& bShouldEnableSpendPointsButton, bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	bShouldEnableSpendPointsButton = false;			
	bShouldEnableEquipButton = false;
	
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpendPointsButton = true;			
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		bShouldEnableEquipButton = false;
		if (SpellPoints > 0)
		{
			bShouldEnableSpendPointsButton = true;			
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpendPointsButton = true;			
		}
	}
}
