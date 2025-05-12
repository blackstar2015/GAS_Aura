// Copyright by Pradeep Pillai



#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IceResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CritChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CritDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CritResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,IceResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,PhysicalResistance, Target, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().IceResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	for(const TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);
		if(TypeDamage > -.5f) //.5 padding for floating point imprecision
		{
			//Determine if there was a successful debuff
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, -1.f);
			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag], EvaluationParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);
			const float EffectiveDebuffChance = SourceDebuffChance * (100.f - TargetDebuffResistance)/100.f;
			const bool bDebuff = FMath::RandRange(1,100) < EffectiveDebuffChance;
			if(bDebuff)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

				UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, true);
				UAuraAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);

				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency, false, -1.f);

				UAuraAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UAuraAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				UAuraAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDef;
	
#pragma region mapping attributes to gameplay tags
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();

	TagsToCaptureDef.Add(Tags.Attributes_Secondary_Armor,DamageStatics().ArmorDef);
	TagsToCaptureDef.Add(Tags.Attributes_Secondary_ArmorPenetration,DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDef.Add(Tags.Attributes_Secondary_BlockChance,DamageStatics().ArmorDef);
	TagsToCaptureDef.Add(Tags.Attributes_Secondary_CritChance,DamageStatics().CritChanceDef);
	TagsToCaptureDef.Add(Tags.Attributes_Secondary_CritDamage,DamageStatics().CritDamageDef);
	TagsToCaptureDef.Add(Tags.Attributes_Secondary_CritResistance,DamageStatics().CritResistanceDef);
	TagsToCaptureDef.Add(Tags.Attributes_Resistance_Fire,DamageStatics().FireResistanceDef);
	TagsToCaptureDef.Add(Tags.Attributes_Resistance_Lightning,DamageStatics().LightningResistanceDef);
	TagsToCaptureDef.Add(Tags.Attributes_Resistance_Ice,DamageStatics().IceResistanceDef);
	TagsToCaptureDef.Add(Tags.Attributes_Resistance_Arcane,DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDef.Add(Tags.Attributes_Resistance_Physical,DamageStatics().PhysicalResistanceDef);
#pragma endregion
	
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar =SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar =TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
#pragma region Debuffs
	DetermineDebuff(ExecutionParams, Spec, EvaluationParameters, TagsToCaptureDef);
#pragma endregion
	
#pragma region Damage
	//Get Damage set by caller magnitude
	float Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		
		checkf(TagsToCaptureDef.Contains(ResistanceTag),
			TEXT("TagsToCaptureDefs doesnt contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDef[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key,false);
		if (DamageTypeValue <= 0.f) continue;
		
		float Resistance = 0.f;		
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef,EvaluationParameters,Resistance);
		Resistance = FMath::Clamp(Resistance,0.f,100.f);

		DamageTypeValue *= (100.f - Resistance)/100.f;

		if (UAuraAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			//Bind Lambda to OnDamageDelegate on the victim here
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar);
			if (CombatInterface)
			{
				CombatInterface->GetOnDamageDelegate().AddLambda([&](float DamageAmount)
				{
					// In Lambda, set DamageTypeValue to the damage received from the broadcast
					DamageTypeValue = DamageAmount;
				});
			}
			/*Call UGameplayStatics::ApplyDamageWithRadialFalloff to cause damage(this will result in TakeDamage being called
			on the victim, which will then broadcast OnDamageDelegate)*/
			UGameplayStatics::ApplyRadialDamageWithFalloff(TargetAvatar,
				DamageTypeValue,
				0.f,
				UAuraAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatar,
				nullptr);
		}
		Damage += DamageTypeValue;		
	}
# pragma endregion

#pragma region BlockChance
	//Capture block chance on target and determine if there was a successful block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluationParameters,TargetBlockChance);
	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* BlockChanceCurve = CharacterClassInfo->
				DamageCalculationCoefficients->FindCurve(FName("BlockChance"),FString());
	const float BlockChanceCoefficient = BlockChanceCurve->Eval(TargetPlayerLevel);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f) * BlockChanceCoefficient;

	//if blocked half the damage
	const bool bBlocked = FMath::RandRange(1.f, 100.f) < TargetBlockChance;
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle,bBlocked);
	Damage = bBlocked? Damage/2 : Damage;
#pragma endregion

#pragma region Armor
	//Armor 
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluationParameters,TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);
	
	//ArmorPenetration
	float SourceArmorPenetration = 0.f; 
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,EvaluationParameters,SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	//armor penetration ignores a percentage of the targets armor
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->
				DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"),FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);
	const float EffectiveArmor = TargetArmor * (100-SourceArmorPenetration * ArmorPenetrationCoefficient)/100.f;

	
	//Armor ignores a percentage of the incoming damage
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve("EffectiveArmor",FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);
	Damage *= (100-EffectiveArmor  * EffectiveArmorCoefficient )/100.f;
#pragma endregion

#pragma region Crits
	//Check CritChance, Crit Damage and Crit Resistance and calculate damage
	float SourceCritChance = 0.f;
	float SourceCritDamage = 0.f;
	float TargetCritResistance = 0.f;
	float RandCritChance = 0.f;
	
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritChanceDef,EvaluationParameters,SourceCritChance);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritDamageDef,EvaluationParameters,SourceCritDamage);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritResistanceDef,EvaluationParameters,TargetCritResistance);
	
	SourceCritChance = FMath::Max<float>(SourceCritChance, 0.f);
	SourceCritDamage = FMath::Max<float>(SourceCritDamage, 0.f);
	TargetCritResistance = FMath::Max<float>(TargetCritResistance, 0.f);
	
	const FRealCurve* CritResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CritResistance"),FString());
	const float CritResistanceCoefficient = CritResistanceCurve->Eval(TargetPlayerLevel);
	const FRealCurve* CritChanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CritChance"),FString());
	const float CritChanceCoefficient = CritChanceCurve->Eval(SourcePlayerLevel);
	
	//Check if Crit and calculate damage accordingly
	RandCritChance = FMath::RandRange(1.f, 100.f);
	const bool bIsCrit = RandCritChance < ((SourceCritChance * CritChanceCoefficient) - (TargetCritResistance * CritResistanceCoefficient));
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle,bIsCrit);
	Damage = bIsCrit ? (Damage*2 + SourceCritDamage) : Damage;
#pragma endregion

#pragma region Halo of Protection
	if (TargetTags->HasTagExact(FAuraGameplayTags::Get().Abilities_Passive_HaloOfProtection))
	{
		Damage = Damage * .15f * EffectContextHandle.GetAbilityLevel();
	}	
#pragma endregion

	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
