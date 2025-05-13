// Copyright by Pradeep Pillai


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
		"<Title> Next Level:</>"
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

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);

	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	const FVector Forward = Rotation.Vector();
	
	if(bUseAbilityLevelForNumProjectiles) NumProjectiles = FMath::Min(GetAbilityLevel(), MaxNumProjectiles);
	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);

	for (const FRotator& Rotator : Rotations)
	{
		float HomingAccelerationMag = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		FVector TargetLocation = HomingTarget->GetActorLocation();
		const float DistanceToTarget = FVector::Dist(SocketLocation, TargetLocation);
		PitchOverride = UAuraAbilitySystemLibrary::CalculateLaunchAngleDegrees(DistanceToTarget, HomingAccelerationMag);
		if (bOverridePitch) Rotation.Pitch = PitchOverride;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotator.Quaternion());

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		
		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile-> HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = HomingAccelerationMag;
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::Min(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;
		
		Projectile->FinishSpawning(SpawnTransform);
	}
	

// Alternate code to calculate speed instead of angle
// FVector SourceLocation = Projectile->DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor()->GetActorLocation();
// FVector TargetLocation = HomingTarget->GetActorLocation();
// const float DistanceToTarget = FVector::Dist(SourceLocation, TargetLocation);
// float Gravity = FMath::Abs(Projectile->ProjectileMovement->GetGravityZ());		
//
// const float DistTimesGrav =  DistanceToTarget * Gravity;
// float PitchRadians = FMath::DegreesToRadians(PitchOverride);
// float PitchSinVal = FMath::Sin(2 * PitchRadians);
//
// // Small non-zero fallback to avoid crash
// if (FMath::IsNearlyZero(PitchSinVal))
// {
// 	UE_LOG(LogTemp, Warning, TEXT("Invalid pitch: sin(2θ) is zero! Check PitchOverride = %f"), PitchOverride);
// 	PitchSinVal = 0.01f;
// }				
//HomingAccelerationMin = FMath::Sqrt(DistTimesGrav / PitchSinVal);
}


