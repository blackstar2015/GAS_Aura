// Copyright by Pradeep Pillai

#pragma once

#include "CoreMinimal.h"
#include "FindInBlueprintManager.h"
#include "GameplayTagContainer.h"

/**
 * Aura GameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FAuraGameplayTags
{
 public:
	static const FAuraGameplayTags& Get() {return GameplayTags;}
	static void InitializeNativeGameplayTags();

	TMap<FGameplayTag,FGameplayTag> DamageTypesToResistances;
	
#pragma region Primary Tags
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigour;
#pragma endregion

#pragma region Secondary Tags
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CritChance;
	FGameplayTag Attributes_Secondary_CritDamage;
	FGameplayTag Attributes_Secondary_CritResistance;
	FGameplayTag Attributes_Secondary_HealthRegen;
	FGameplayTag Attributes_Secondary_ManaRegen;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
#pragma endregion
	
#pragma region Meta Attribute Tags
	FGameplayTag Attributes_Meta_IncomingXP;
#pragma endregion

#pragma region Resistances
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Ice;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;
#pragma endregion
	
#pragma region Input Tags
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;
#pragma endregion
	
#pragma region Damage Tags
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Ice;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
#pragma endregion
	
#pragma region Abilities Tags
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_Fire_FireBolt;
#pragma endregion
	
#pragma region Cooldown Tags
	FGameplayTag Cooldown_Fire_FireBolt;
#pragma endregion

	
#pragma region Combat Socket Tags
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;
#pragma endregion
	
#pragma region Attack Montage tags
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
#pragma endregion

#pragma region Effect Tags
	FGameplayTag Effects_HitReact;
#pragma endregion
	
private:
	static FAuraGameplayTags GameplayTags;
		
};
