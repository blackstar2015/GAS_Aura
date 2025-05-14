// Copyright by Pradeep Pillai

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UGameplayEffect;
class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;\

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn=true))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnHit();
	virtual void Destroyed() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<USphereComponent> Sphere;
	
	UFUNCTION()
	virtual void OnSphereOverlapped(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	bool IsValidOverlap(AActor* OtherActor);
	
	bool bHit = false;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;	
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;

};
