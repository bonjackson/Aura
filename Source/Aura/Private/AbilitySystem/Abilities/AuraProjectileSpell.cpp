


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"


#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		FTransform SpawnTransform;
		
		FVector SocketLoc = CombatInterface->GetCombatSocketLocation();
		// 【添加这句日志】看看到底在哪个坐标生成的！
		UE_LOG(LogTemp, Warning, TEXT("==== 准备生成火球，目标坐标是: %s ===="), *SocketLoc.ToString());
		
		
		SpawnTransform.SetLocation(CombatInterface->GetCombatSocketLocation());
		SpawnTransform.SetRotation(GetAvatarActorFromActorInfo()->GetActorQuat());
		//SpawnActorDeferred将异步创建实例，在实例创建完成时，相应的数据已经应用到了实例身上
		AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		//确保变换设置被正确应用
		Projectile->FinishSpawning(SpawnTransform);
	}
	
}
