


#include "Character/AuraCharacter.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
DEFINE_LOG_CATEGORY_STATIC(LogAuraCharacter, All, All);
AAuraCharacter::AAuraCharacter()
{
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	GetCharacterMovement()->bConstrainToPlane=true;
	GetCharacterMovement()->bSnapToPlaneAtStart=true;
	
	bUseControllerRotationPitch=false;
	bUseControllerRotationRoll=false;
	bUseControllerRotationYaw=false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//Init ability actor info in the server
	InitAbilityActorInfo();

}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	//Init ability actor info for the client
	InitAbilityActorInfo();
}
void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState*AuraPlayerState=GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	const FString RoleString = GetLocalRole() == ROLE_Authority ? TEXT("SERVER") : TEXT("CLIENT");

	if (AuraPlayerState)
	{
		UE_LOG(LogAuraCharacter, Warning, TEXT("[%s] Calling InitAbilityActorInfo. PlayerState is valid."), *RoleString);
	}
	else
	{
		// 如果 PlayerState 是空的，这是个严重问题
		UE_LOG(LogAuraCharacter, Error, TEXT("[%s] FAILED to get AuraPlayerState in InitAbilityActorInfo!"), *RoleString);
		return;
	}
	
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	AbilitySystemComponent=AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet=AuraPlayerState->GetAttributeSet();
	if (AbilitySystemComponent && AttributeSet)
	{
		UE_LOG(LogAuraCharacter, Warning, TEXT("[%s] InitAbilityActorInfo SUCCESS. ASC and AttributeSet are valid."), *RoleString);
	}
	else
	{
		UE_LOG(LogAuraCharacter, Error, TEXT("[%s] InitAbilityActorInfo FAILED. ASC or AttributeSet is NULL."), *RoleString);
	}
}
