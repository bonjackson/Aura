


#include "Actor/Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere); //设置其为根节点，
	Sphere->SetCollisionObjectType(ECC_PROJECTILE);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //设置其只用作查询使用
	Sphere->SetCollisionResponseToChannels(ECR_Ignore); //设置其忽略所有碰撞检测
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap); //设置其与世界动态物体产生重叠事件
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap); //设置其与世界静态物体产生重叠事件
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); //设置其与Pawn类型物体产生重叠事件
	
	//创建发射组件
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f; //设置初始速度
	ProjectileMovement->MaxSpeed = 550.f; //设置最大速度
	ProjectileMovement->ProjectileGravityScale = 0.f; //设置重力影响因子，0为不受影响
	


																												
}

void AProjectile::Destroyed()
{
	if(!bHit && !HasAuthority())
	{
		PlayImpact();
	}
	Super::Destroyed();
}

void AProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayImpact();

	//在重叠后，销毁自身
	if(HasAuthority())
	{
		if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectHandle.Data.Get());
		}
		Destroy();
	}
	else
	{
		//如果对actor没有权威性，将bHit设置为true，证明当前已经播放了击中特效
		bHit = true;
	}
}

void AProjectile::PlayImpact() const
{
	//播放音效
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	//播放粒子特效
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	
	//将音乐停止后会自动销毁
	if(LoopingSoundComponent) LoopingSoundComponent->Stop();
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnSphereOverlap);
	
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

