// Copyright by Pradeep Pillai


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Cant find info for ability tag [%s] on abilityInfo [%s]"),
			*AbilityTag.ToString(), *AbilityTag.ToString(), *GetNameSafe(this));
	}	
	return  FAuraAbilityInfo();
}
