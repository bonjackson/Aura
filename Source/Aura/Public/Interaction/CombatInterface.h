

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
//蒙太奇动画和标签以及骨骼位置的映射，用于攻击技能获取和设置攻击范围
USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	//使用的蒙太奇
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	//对应的标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

	//攻击时的触发伤害的骨骼插槽
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName CombatTipSocketName; //设置技能释放的位置
};

class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	
public:
	virtual int32 GetPlayerLevel();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FVector GetCombatSocketLocation() const;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage(); //获取受击蒙太奇动画
	
	virtual void Die() = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const; //获取当前角色是否死亡

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar(); //获取当前角色
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocationByStruct(const FTaggedMontage TaggedMontage) const;


protected:
	
};
