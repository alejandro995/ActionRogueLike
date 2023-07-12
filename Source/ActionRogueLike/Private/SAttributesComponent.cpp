// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributesComponent.h"

#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("SU.DamageMultipier"), 1.0f, TEXT("Global Damage Modifier for Atribute Component"), ECVF_Cheat);

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
	if(!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}
	
	float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
		GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

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


