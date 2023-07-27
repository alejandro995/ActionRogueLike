// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Actions/SActionComponent.h"
#include "Components/SphereComponent.h"
#include "FunctionLibrary/SGamePlayFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SActionEffect.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	InitialLifeSpan = 10.0f;
	DamageAmount = 20.0f;
	DamageAmount = 20.0f;
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}


void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                       bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetInstigator())
	{
		

		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if(ActionComp && ActionComp->ActiveGamePlayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		
		if (USGamePlayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();

			if (ActionComp && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
		}
	}
}


