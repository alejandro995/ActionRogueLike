// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SAttributesComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->OnComponentHit.AddDynamic(this, &ASMagicProjectile::OnActorHit);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor && OtherActor != GetInstigator())
	{

		UE_LOG(LogTemp, Warning, TEXT("FUCK I EXPODEEEE MAGIC PROJECTILE"));
		
		USoundBase* ImpactSound = ImpactAudioComp->GetSound();
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound, SphereComp->GetComponentLocation());
		//UGameplayStatics::PlayWorldCameraShake()

		USAttributesComponent* AtrributeComp = Cast<USAttributesComponent>(OtherActor->GetComponentByClass(USAttributesComponent::StaticClass()));
		if(AtrributeComp)
		{
			AtrributeComp->ApplyHealthChange(-20.0f);

			Destroy();
		}
	}
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                       bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("FUCK I EXPODEEEE MAGIC PROJECTILE OVERLAP"));
	if(OtherActor && OtherActor != GetInstigator())
	{
		USAttributesComponent* AtrributeComp = Cast<USAttributesComponent>(OtherActor->GetComponentByClass(USAttributesComponent::StaticClass()));
		if(AtrributeComp)
		{
			AtrributeComp->ApplyHealthChange(-20.0f);

			Destroy();
		}
	}
	
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	GetWorldTimerManager().SetTimer(TimerHandle_IgnoreInstigator, this, &ASMagicProjectile::ActivateInstigatorCollition, 0.2f);
}

void ASMagicProjectile::ActivateInstigatorCollition()
{
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), false);
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

