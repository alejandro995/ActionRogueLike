// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASDashProjectile::ASDashProjectile()
{
	ExplotionEffectComp = CreateDefaultSubobject<UParticleSystemComponent>("ExplotionEffectComp");
	ExplotionEffectComp->SetupAttachment(SphereComp);
	ExplotionEffectComp->Deactivate();
	
 	//Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(this->GetInstigator(), true);
	
	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ASDashProjectile::Explode_TimeElapsed, 0.5f);
	
}

void ASDashProjectile::Explode_TimeElapsed()
{
	MovementComp->StopMovementImmediately();
	EffectComp->Deactivate();
	ExplotionEffectComp->Activate();
	

	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ASDashProjectile::Teleport_TimeElapsed, 0.2f);
	
}

void ASDashProjectile::Teleport_TimeElapsed()
{

	AActor* ActorToTeleport = GetInstigator();
	FVector TeleportLocation = SphereComp->GetComponentLocation();
	DrawDebugSphere(GetWorld(), TeleportLocation, 10, 16, FColor::Orange, false, 2.0f);
	//this->GetInstigator();

	if(ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(TeleportLocation, ActorToTeleport->GetActorRotation(), false, false);
	}
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(this->GetInstigator()), GetWorld()->TimeSeconds);
	
	
	
}


// Called every frame
void ASDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

