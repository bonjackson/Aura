


#include "AbilitySystem/AuraAbilitySystemGlobals.h"

#include "RPGAbilityTypes.h"

FGameplayEffectContext* UAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FRPGGameplayEffectContext();
}
