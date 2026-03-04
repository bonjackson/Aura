

#pragma once

#include "CoreMinimal.h"
#include "UI/Controller/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"
struct FOnAttributeChangeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanagedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChanagedSignature,float,NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChanagedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChanagedSignature,float,NewMana);
/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadCastInitialValues() override;
	virtual  void BindCallbackToDepencies() override;
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnHealthChanagedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnMaxHealthChanagedSignature OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnManaChanagedSignature OnManaChanged;
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnMaxManaChanagedSignature OnMaxManaChanged;
protected:
	void HealthChanged(const FOnAttributeChangeData &Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData &Data) const;
	void ManaChanged(const FOnAttributeChangeData &Data) const;
	void MaxManaChanged(const FOnAttributeChangeData &Data) const;
};
