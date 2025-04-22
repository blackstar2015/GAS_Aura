// Copyright by Pradeep Pillai

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage")
	FScalableFloat BaseDamage;

	//TODO: You can make these scalable floats
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffChance = 20.f;

	//You can randomize these and apply level curves to these as well
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffDamage = 5.f;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffDuration = 5.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Damage Forces")
	float DeathImpulseMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Damage Forces")
	float KnockBackForceMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Damage Forces")
	float KnockBackChance = 0.f;
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
};
