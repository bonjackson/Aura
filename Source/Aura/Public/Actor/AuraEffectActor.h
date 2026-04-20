

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"


class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy
{
	RemoveOnOverlap,
	RemoveOnEndOverlap,
	DoNotRemove
};
UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AAuraEffectActor();

protected:

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect>GameplayEffectClass);
	
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
	
	//Instant和Duration的GE在应用后，此物体是否需要被销毁
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apply Effects")
	bool bDestroyOnEffectApplication = true;

	//敌人是否能够拾取此物体
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apply Effects")
	bool bApplyEffectsToEnemies = false;
	
	UPROPERTY(EditAnywhere,Category="Applied Effect",BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	
	UPROPERTY(EditAnywhere,Category="Applied Effect",BlueprintReadOnly)
	EEffectApplicationPolicy InstantGameplayEffectPolicy=EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere,Category="Applied Effect",BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	
	UPROPERTY(EditAnywhere,Category="Applied Effect",BlueprintReadOnly)
	EEffectApplicationPolicy DurationGameplayEffectPolicy=EEffectApplicationPolicy::DoNotApply;;
	
	UPROPERTY(EditAnywhere,Category="Applied Effect",BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	
	UPROPERTY(EditAnywhere,Category="Applied Effect",BlueprintReadOnly)
	EEffectApplicationPolicy InfiniteGameplayEffectPolicy=EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere,Category="Applied Effect",BlueprintReadOnly)
	EEffectRemovalPolicy InfiniteGameplayEffectRemovalPolicy= EEffectRemovalPolicy::RemoveOnEndOverlap;

	TMap<FActiveGameplayEffectHandle,UAbilitySystemComponent*>ActiveEffectHandles;
	
	UPROPERTY(EditAnywhere,Category="Applied Effect",BlueprintReadOnly)
	float ActorLevel=1.f;
};