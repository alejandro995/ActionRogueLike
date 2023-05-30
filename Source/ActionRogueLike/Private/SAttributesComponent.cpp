// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributesComponent.h"

// Sets default values for this component's properties
USAttributesComponent::USAttributesComponent()
{
	Health = FMath::Clamp(100, 0, 100);
	MaxHealth = FMath::Clamp(100, 0, 100);
}

bool USAttributesComponent::isAlive() const
{
	return Health > 0.0f;
}

bool USAttributesComponent::isFullHealth() const
{
	return Health == MaxHealth;
}

bool USAttributesComponent::ApplyHealthChange(AActor* InstigatorActor,  float Delta)
{
	float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	return ActualDelta != 0;
}


