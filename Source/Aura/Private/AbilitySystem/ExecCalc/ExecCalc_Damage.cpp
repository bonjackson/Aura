


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AuraGameplayTags.h"
#include "RPGAbilityTypes.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//存储标签和属性快照对应的Map
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	//添加标签和属性快照对应的数据
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Fire, DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Lightning, DamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);
	
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	//获取AvatarActor
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	//获取到战斗接口
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);
	
	//获取挂载此类的GE实例
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	//设置评估参数
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	//获取到角色配置数据
	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	
	//获取伤害数值
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage,false,0.f);
	
	for(const TTuple<FGameplayTag, FGameplayTag>& Pair : GameplayTags.DamageTypesToResistance)
	{
		const FGameplayTag DamageType = Pair.Key;
		const FGameplayTag ResistanceType = Pair.Value;
		//检查对应的属性快照是否设置，防止报错
		checkf(TagsToCaptureDefs.Contains(ResistanceType), TEXT("在ExecCalc_Damage中，无法获取到Tag[%s]对应的属性快照"), *ResistanceType.ToString());
		//通过抗性标签获取到属性快照
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceType];
		//获取抗性值
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f); //将抗住限制在0到100
		//通过Tag获取对应伤害类型的值，如果没设置SetByCaller将获取0
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageType,false,0.f);
		//通过抗性计算出能够对角色造成的伤害值
		DamageTypeValue *= (100.f - Resistance) / 100.f;
		//将每种属性伤害值合并进行后续计算
		Damage += DamageTypeValue;
	}
	
	//获取格挡率，如果触发格挡，伤害减少一半
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max(0.f, TargetBlockChance);
	//根据格挡概率判断当前是否触发
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	
	//获取GE的上下文句柄
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	
	UAuraAbilitySystemLibrary::SetIsBlockHit(EffectContextHandle,bBlocked);

	
	if(bBlocked) Damage *= 0.5f;

	//--------------------处理目标护甲和源的护甲穿透影响伤害--------------------
	//获取目标护甲值
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max(0.f, TargetArmor);
	//获取源护甲穿透
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(0.f, SourceArmorPenetration);
	//获取到数据表内的护甲穿透系数
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	//护甲穿透将按照比例忽略目标的护甲值，护甲穿透的值可以根据某个方程计算出实际护甲穿透率（可以根据等级，等级越高，每一点护甲穿透值的比例越低）
	const float EffectiveArmor = TargetArmor * (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	//获取到数据表内的护甲系数
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	//忽略后的护甲值将以一定比例影响伤害
	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	//--------------------暴击伤害--------------------
	//暴击率
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max(0.f, SourceCriticalHitChance);
	//暴击伤害
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max(0.f, SourceCriticalHitDamage);
	//暴击抵抗
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max(0.f, TargetCriticalHitResistance);
	//获取到数据表内的暴击抵抗系数
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	//计算当前是否暴击
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
	
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle,bCriticalHit);
	
	//触发暴击 伤害乘以暴击伤害率
	if(bCriticalHit) Damage = Damage * 2.f + SourceCriticalHitDamage;

	//输出计算结果
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
