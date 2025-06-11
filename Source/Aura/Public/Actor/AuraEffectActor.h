// Copyright by Pradeep Pillai

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy: uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply	
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy: uint8
{
	RemoveOnEndOverlap,
	DoNotRemove	
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

	
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);	
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void StartSinusoidalMovement();
	UFUNCTION(BlueprintCallable)
	void StartRotation();
	
	UPROPERTY(BlueprintReadWrite)
	FVector CalculatedLocation;
	UPROPERTY(BlueprintReadWrite)
	FRotator CalculatedRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupMovement")
	bool bRotates = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupMovement")
	bool bSinusoidalMovement = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupMovement")
	float RotationRate = 45.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupMovement")
	float SineAmplitude = 	1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupMovement")
	float SinePeriodConstant = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupMovement")
	FVector InitialLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	bool bDestroyOnEffectApplication = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	bool bApplyEffectToEnemies = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	float ActorLevel = 1.f;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effects|Instant")
	TSubclassOf<UGameplayEffect>InstantGameplayEffectClass;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effects|Instant")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effects|Duration")
	TSubclassOf<UGameplayEffect>DurationGameplayEffectClass;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effects|Duration")
	EEffectApplicationPolicy DurationEffectApplicationPolicy= EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effects|Infinite")
	TSubclassOf<UGameplayEffect>InfiniteGameplayEffectClass;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effects|Infinite")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy= EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effects|Infinite")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	TMap<FActiveGameplayEffectHandle,UAbilitySystemComponent*> ActiveEffectHandles;

private:
	float RunningTime = 1.f;
	void ItemMovement(float DeltaTime);
	
};
