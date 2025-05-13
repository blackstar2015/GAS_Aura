// Copyright by Pradeep Pillai


#include "AbilitySystem/Abilities/AuraArcaneShards.h"

FString UAuraArcaneShards::GetDescription(int32 Level)
{
	const int32 Damage = BaseDamage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 NumberOfShards = (Level-1) * 2 + 1;
	if(Level == 1)
	{
		return 	FString::Printf(TEXT(
			"<Title> Arcane Shards</>"
			"\n\n<Small>Current Level: </><Level>%d</>"
			"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
			"\n<Small>Cooldown: </><Cooldown>%.1f</>"
			"\n\n<Default>Spawns an arcane shard centered around the chosen target dealing: "
			"</><Damage>%d</><Default> Arcane damage knocking back all enemies in the area.</>"
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
			"<Title> Arcane Shards</>"
			"\n\n<Small>Current Level: </><Level>%d</>"
			"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
			"\n<Small>Cooldown: </><Cooldown>%.1f</>"
			"\n\n<Default>Spawns %d arcane shards centered around the chosen target dealing: "
			"</><Damage>%d</><Default> Arcane damage knocking back all enemies in the area.</>"
			),
			 Level,
			 ManaCost,
			 Cooldown,
			 FMath::Min(NumberOfShards,MaxNumShards),
			 Damage);
	}
}

FString UAuraArcaneShards::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = BaseDamage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 NumberOfShards = (Level-1)*2 + 1;

	return FString::Printf(TEXT(
		"<Title> Next Level:</>"
		"\n\n<Small>Next Level: </><Level>%d</>"
		"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
		"\n<Small>Cooldown: </><Cooldown>%.1f</>"
		"\n\n<Default>Spawns %d arcane shards centered around the chosen target dealing: "
		"</><Damage>%d</><Default> Arcane damage knocking back all enemies in the area.</>"
		),
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(NumberOfShards,MaxNumShards),
		Damage);
}
