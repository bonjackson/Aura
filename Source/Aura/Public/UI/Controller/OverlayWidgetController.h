

#pragma once

#include "CoreMinimal.h"
#include "UI/Controller/AuraWidgetController.h"

#include "OverlayWidgetController.generated.h"

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MessageTag=FGameplayTag();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText Message =FText();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<class UAuraUserWidget> MessegeWidget;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* Image =nullptr;
	
};
class UAuraUserWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributChanagedSignature, float, Newvalue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow,Row);

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
	FOnAttributChanagedSignature OnHealthChanged;
	
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributChanagedSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributChanagedSignature OnManaChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributChanagedSignature OnMaxManaChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Widget Data")
	TObjectPtr<UDataTable>MessageWidgetDataTable;
	

	
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable,const FGameplayTag& Tag);
	

};
template<typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable,const FGameplayTag& Tag)
{
		return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}