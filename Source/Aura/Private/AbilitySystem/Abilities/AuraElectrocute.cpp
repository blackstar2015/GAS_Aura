// Copyright by Pradeep Pillai


#include "AbilitySystem/Abilities/AuraElectrocute.h"

FString UAuraElectrocute::GetDescription(int32 Level)
{
	const int32 Damage = BaseDamage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if(Level == 1)
	{
		return 	FString::Printf(TEXT(
			"<Title> Electrocute</>"
			"\n\n<Small>Current Level: </><Level>%d</>"
			"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
			"\n<Small>Cooldown: </><Cooldown>%.1f</>"
			"\n\n<Default>Launch a continuous beam of electricity at the target dealing: "
			"</><Damage>%d</><Default> lightning damage per second with a chance to stun.</>"
			),
			 Level,
			 ManaCost,
			 Cooldown,
			 Damage
			 );
	}
	else if(Level == 2)
	{		
		return 	FString::Printf(TEXT(
			"<Title> Electrocute</>"
			"\n\n<Small>Current Level: </><Level>%d</>"
			"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
			"\n<Small>Cooldown: </><Cooldown>%.1f</>"
			"\n\n<Default>Launch a continuous beam of electricity at the target dealing: "
			"</><Damage>%d</><Default> lightning damage per second and chaining to %d additional target with a chance to stun.</>"
			),
			 Level,
			 ManaCost,
			 Cooldown,
			 Damage,
			 FMath::Min(Level,MaxNumChainLightning));
	}
	else
	{		
		return 	FString::Printf(TEXT(
			"<Title> Electrocute</>"
			"\n\n<Small>Current Level: </><Level>%d</>"
			"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
			"\n<Small>Cooldown: </><Cooldown>%.1f</>"
			"\n\n<Default>Launch a continuous beam of electricity at the target dealing: "
			"</><Damage>%d</><Default> lightning damage per second and chaining to %d additional targets with a chance to stun.</>"
			),
			 Level,
			 ManaCost,
			 Cooldown,
			 Damage,
			 FMath::Min(Level,MaxNumChainLightning));
	}
}

FString UAuraElectrocute::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = BaseDamage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return 	FString::Printf(TEXT(
			"<Title> Next Level:</>"
			"\n\n<Small>Current Level: </><Level>%d</>"
			"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
			"\n<Small>Cooldown: </><Cooldown>%.1f</>"
			"\n\n<Default>Launch a continuous beam of electricity at the target dealing: "
			"</><Damage>%d</><Default> lightning damage per second and chaining to %d additional targets with a chance to stun.</>"
			),
			 Level,
			 ManaCost,
			 Cooldown,
			 Damage,
			 FMath::Min(Level,MaxNumChainLightning));
}
