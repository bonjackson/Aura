

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AuraTargetDataUnderMouse.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);
UCLASS()
class AURA_API UAuraTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAuraTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);
	
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;
	
private:

	virtual void Activate() override;
	
	//客户端向服务器端提交数据
	void SendMouseCursorData();
	
	//当数据提交到服务器端后的委托回调
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);

};
