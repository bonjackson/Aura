


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"


#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority(); //判断此函数是否在服务器运行
	if (!bIsServer) return;
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo());
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation(); //将方向转为旋转
		//Rotation.Pitch = 0.f; //设置Pitch为0，转向的朝向将平行于地面

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo()));
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
		//SpawnActorDeferred将异步创建实例，在实例创建完成时，相应的数据已经应用到了实例身上
		AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		//创建一个GE的实例，并设置给投射物
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this); //设置技能
		EffectContextHandle.AddSourceObject(Projectile); //设置GE的源
		
		//添加Actor列表
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		//添加命中结果
		FHitResult HitResult;
		HitResult.Location = ProjectileTargetLocation;
		EffectContextHandle.AddHitResult(HitResult);
		//添加技能触发位置
		EffectContextHandle.AddOrigin(ProjectileTargetLocation);
		
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get(); //获取标签单例
		//UAbilitySystemBlueprintLibrary::AssignSetByCallerMagnitude() //使用DataName设置
		for(auto& Pair : DamageTypes)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel()); //根据等级获取技能伤害
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
		}
		
		Projectile->DamageEffectHandle = SpecHandle;
		//确保变换设置被正确应用
		Projectile->FinishSpawning(SpawnTransform);
	}
}
