// Copyright by Pradeep Pillai

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

# pragma region CombatInterface
	virtual int32 GetPlayerLevel() override;
#pragma endregion
	
# pragma region PlayerInterface
	virtual void AddToXP_Implementation(int32 InXP) override;
#pragma endregion
	
protected:
	virtual void InitAbilityActorInfo() override;
};
