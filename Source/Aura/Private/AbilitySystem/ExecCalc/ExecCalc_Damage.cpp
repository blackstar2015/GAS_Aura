// Copyright by Pradeep Pillai



#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritResistance);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CritChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CritDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CritResistance, Target, false);
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
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar =SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar =TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
#pragma region Damage
	//Get Damage set by caller magnitude
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	float RandDamageMultiplier = FMath::RandRange(-1.0f, 1.0f);
	Damage = Damage - RandDamageMultiplier; 
# pragma endregion

#pragma region BlockChance
	//Capture block chance on target and determine if there was a successful block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluationParameters,TargetBlockChance);
	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* BlockChanceCurve = CharacterClassInfo->
				DamageCalculationCoefficients->FindCurve(FName("BlockChance"),FString());
	const float BlockChanceCoefficient = BlockChanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f) * BlockChanceCoefficient;

	//if blocked half the damage
	const bool bBlocked = FMath::RandRange(1.f, 100.f) < TargetBlockChance;
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
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	const float EffectiveArmor = TargetArmor * (100-SourceArmorPenetration * ArmorPenetrationCoefficient)/100.f;

	
	//Armor ignores a percentage of the incoming damage
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve("EffectiveArmor",FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
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
	const float CritResistanceCoefficient = CritResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	const FRealCurve* CritChanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CritChance"),FString());
	const float CritChanceCoefficient = CritChanceCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	//Check if Crit and calculate damage accordingly
	RandCritChance = FMath::RandRange(1.f, 100.f);
	const bool bIsCrit = RandCritChance < ((SourceCritChance * CritChanceCoefficient) - (TargetCritResistance * CritResistanceCoefficient));
	Damage = bIsCrit ? (Damage*2 + SourceCritDamage) : Damage;
#pragma endregion

	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
