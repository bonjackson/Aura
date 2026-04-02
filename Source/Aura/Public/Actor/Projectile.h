

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"


class UNiagaraSystem;

UCLASS()
class AURA_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;
	
	void Destroyed() override;
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void PlayImpact() const;
	
	bool bHit;
	
	//移动循环音效
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;

	//储存循环音效的变量，后续用于删除
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
	
public:	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
private:

	//此物体的存在时间
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;
};
