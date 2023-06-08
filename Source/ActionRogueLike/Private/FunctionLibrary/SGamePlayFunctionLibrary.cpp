// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/SGamePlayFunctionLibrary.h"

#include "SAttributesComponent.h"

bool USGamePlayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributesComponent* AttributesComponent = USAttributesComponent::GetAttributes(TargetActor);

	if (AttributesComponent)
	{
		return AttributesComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
		
	}

	return false;
}

bool USGamePlayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();

		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			FVector DirectionVector  = HitResult.TraceEnd - HitResult.TraceStart;
			DirectionVector.Normalize();
			
			HitComp->AddImpulseAtLocation(DirectionVector * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);	
		}
		return true;
	}

	return false;
}

