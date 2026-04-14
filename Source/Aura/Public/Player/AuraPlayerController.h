

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UDamageTextComponent;
class USplineComponent;
class UAuraAbilitySystemComponent;
class UInputConfig;
struct FGameplayTag;
/**
 * 
 */
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter,bool IsBlockedHit, bool IsCriticalHit); //在每个客户端显示伤害数值
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

private:

	UPROPERTY(EditAnywhere,Category="Input");
	TObjectPtr<UInputMappingContext> AuraContext;
	
	UPROPERTY(EditAnywhere,Category="Input");
	TObjectPtr<UInputAction>MoveAction;
	void Move(const FInputActionValue&InputActionValue);
	
	void CursorTrace();
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	FHitResult CursorHit;
	
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHold(FGameplayTag InputTag);
	
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputConfig> InputConfig;
	
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponentBase;

	UAuraAbilitySystemComponent* GetASC();
	
	
	FVector CachedDestination = FVector::ZeroVector; //存储鼠标点击的位置
	float FollowTime = 0.f; // 用于查看按住了多久
	bool bAutoRunning = false; //当前是否自动移动
	bool bTargeting = false; //当前鼠标是否选中敌人

	UPROPERTY(EditDefaultsOnly)
	float ShortPressThreshold = 0.3f; //定义鼠标悬停多长时间内算点击事件

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f; //当角色和目标距离在此半径内时，将关闭自动寻路

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline; //自动寻路时生成的样条线
	
	void AutoRun();
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction;
	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;
};
