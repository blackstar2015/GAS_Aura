// Copyright by Pradeep Pillai

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,bool bOverridePitch = false, float PitchOverride = 0.f, AActor* HomingTarget = nullptr);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	int32 MaxNumProjectiles = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float HomingAccelerationMin = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float HomingAccelerationMax = 3200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float Offset = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float MinDistanceForHoming = 300.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	bool bLaunchHomingProjectiles = true;
};
