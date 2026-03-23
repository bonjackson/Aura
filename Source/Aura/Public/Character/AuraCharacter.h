

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "AuraCharacter.generated.h"
/**
 * 
 */
UCLASS()
class AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	//Combat Interface
	virtual  int32 GetPlayerLevel() override;
private:
	void InitAbilityActorInfo() override;
};
