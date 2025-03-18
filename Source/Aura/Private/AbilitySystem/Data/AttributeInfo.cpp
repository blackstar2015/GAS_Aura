// Copyright by Pradeep Pillai


#include "AbilitySystem/Data/AttributeInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for(const FAuraAttributeInfo& Info: AttributeInformation)
	{
		if(Info.AttributeTag == AttributeTag)
		{
			return Info;
		}
	}
	if(bLogNotFound)
	{
		UE_LOG(LogAura,Error, TEXT("Cant find AttributeTag [%s] on attributeInfo [%s]"),
			*AttributeTag.ToString(),*GetNameSafe(this))
	}
	return FAuraAttributeInfo();
}
