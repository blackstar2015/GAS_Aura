// Copyright by Pradeep Pillai


#include "AbilitySystem/Abilities/AuraFireBolt.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 Damage = BaseDamage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	if(Level == 1)
	{
		return 	FString::Printf(TEXT(
			"<Title> Fire Bolt</>"
			"\n\n<Small>Current Level: </><Level>%d</>"
			"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
			"\n<Small>Cooldown: </><Cooldown>%.1f</>"
			"\n\n<Default>Launches a bolt of fire,exploding on impact and dealing: "
			"</><Damage>%d</><Default> fire damage with a chance to burn.</>"
			),
			 Level,
			 ManaCost,
			 Cooldown,
			 Damage
			 );
	}
	else
	{		
		return 	FString::Printf(TEXT(
			"<Title> Fire Bolt</>"
			"\n\n<Small>Current Level: </><Level>%d</>"
			"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
			"\n<Small>Cooldown: </><Cooldown>%.1f</>"
			"\n\n<Default>Launches %d bolts of fire,exploding on impact and dealing: "
			"</><Damage>%d</><Default> fire damage with a chance to burn.</>"
			),
			 Level,
			 ManaCost,
			 Cooldown,
			 FMath::Min(Level,NumProjectiles),
			 Damage);
	}

}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = BaseDamage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title> Fire Bolt</>"
		"\n\n<Small>Next Level: </><Level>%d</>"
		"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
		"\n<Small>Cooldown: </><Cooldown>%.1f</>"
		"\n\n<Default>Launches %d bolts of fire,exploding on impact and dealing: "
		"</><Damage>%d</><Default> fire damage with a chance to burn.</>"
		),
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level,NumProjectiles),
		Damage);
}
