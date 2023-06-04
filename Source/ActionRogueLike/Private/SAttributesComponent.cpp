// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributesComponent.h"

// Sets default values for this component's properties
USAttributesComponent::USAttributesComponent()
{
	Health = FMath::Clamp(100, 0, 100);
	MaxHealth = FMath::Clamp(100, 0, 100);
	LowHealthTreshold = 33;
}

bool USAttributesComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

bool USAttributesComponent::isAlive() const
{
	return Health > 0.0f;
}

bool USAttributesComponent::isFullHealth() const
{
	return Health == MaxHealth;
}

bool USAttributesComponent::isLowHealth() const
{
	return Health <= 33  ;
}

float USAttributesComponent::GetMaxHealth() const
{
	return MaxHealth;
}

bool USAttributesComponent::ApplyHealthChange(AActor* InstigatorActor,  float Delta)
{
	if(!GetOwner()->CanBeDamaged())
	{
		return false;
	}
	float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	return ActualDelta != 0;
}

USAttributesComponent* USAttributesComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributesComponent>(FromActor->GetComponentByClass(USAttributesComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributesComponent::IsActorAlive(AActor* Actor)
{
	USAttributesComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->isAlive();
	}

	return false;
}

bool USAttributesComponent::IsActorLowHealth(AActor* Actor)
{
	USAttributesComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->isLowHealth();
	}

	return false;
}


