 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"

#include "AuraCharacterBase.generated.h"

class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;
UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter,public IAbilitySystemInterface,public ICombatInterface
{
	GENERATED_BODY()

public:
	
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent()const override;
	UAttributeSet*GetAttributeSet()const{return AttributeSet; }; 
protected:

	virtual void BeginPlay() override;
	
	bool bDead = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	virtual void InitAbilityActorInfo();
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect>GameplayEffectClass,float Level) const;
	virtual void InitializeDefaultAttributes() const;
	
	void AddCharacterAbilities();
	
	
	
	/* ICombatInterface战斗接口 */
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual FVector GetCombatSocketLocation_Implementation() const override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual FVector GetCombatSocketLocationByStruct_Implementation(const FTaggedMontage TaggedMontage) const override;
	virtual void Die() override;
	/* ICombatInterface战斗接口 结束 */
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontage;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	
	void Dissolve(); //溶解效果
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(const TArray<UMaterialInstanceDynamic*>& DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UNiagaraSystem* BloodEffect; //受伤特效
private:

	
	UPROPERTY(EditAnywhere, Category="Attributes")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities; 
	

	
};
 