


#include "Player/AuraPlayerController.h"
#include"EnhancedInputSubsystems.h"
#include"EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates=true;
	
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	cursorTrace();
}
void AAuraPlayerController::cursorTrace()
{
	FHitResult CurSorHit;
	GetHitResultUnderCursor(ECC_Visibility,false,CurSorHit);
	if (!CurSorHit.bBlockingHit) return;
	if (CurSorHit.bBlockingHit && GEngine)
	{
		FString ActorName = CurSorHit.GetActor()->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Cursor Hit: %s"), *ActorName));
	}
	LastActor=ThisActor;
	ThisActor=Cast<IEnemyInterface>(CurSorHit.GetActor());
	
	if (LastActor==nullptr)
	{
		if (ThisActor!=nullptr)
		{
			ThisActor->HightlightActor();
		}
		else
		{
			//do nothing
		}
	}
	else
	{
		if (ThisActor==nullptr)
		{
			LastActor->UnHightlightActor();
		}
		else
		{
			if (LastActor!=ThisActor)
			{
				LastActor->UnHightlightActor();
				ThisActor->HightlightActor();
			}
			else
			{	
				//do nothing
			}
		}
	}
}
void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext,0);
	}
	
	
	bShowMouseCursor=true;
	DefaultMouseCursor=EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent*EnhancedInputComponent=CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
	
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation=GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);
	
	const FVector ForwardDirection =FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection =FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* ControlledPawn =GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
}


