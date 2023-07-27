// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributesComponent.h"

#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("SU.DamageMultipier"), 1.0f, TEXT("Global Damage Modifier for Atribute Component"), ECVF_Cheat);

// Sets default values for this component's properties
USAttributesComponent::USAttributesComponent()
{
	Health = FMath::Clamp(100, 0, 100);
	MaxHealth = FMath::Clamp(100, 0, 100);
	Rage = FMath::Clamp(0, 0, 100);
	MaxRage = FMath::Clamp(100, 0, 100);
	LowHealthTreshold = 33;

	SetIsReplicatedByDefault(true);
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

float USAttributesComponent::getRage() const
{
	return Rage;
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
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	float ActualDelta = NewHealth - OldHealth;

	//Is Server?
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if(ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);

		}

		if (ActualDelta < 0.0f && Health == 0.0f)
		{
			ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	
	return ActualDelta != 0;
}

bool USAttributesComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	 float OldRage = Rage;
	 Rage = FMath::Clamp(Rage + Delta, 0.0f, MaxRage);

	float ActualDelta = Rage - OldRage;
	 if (ActualDelta != 0.0f)
	 {
	 	MulticastRageChanged(InstigatorActor, Rage, ActualDelta);
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

void USAttributesComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributesComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

void USAttributesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributesComponent, Health);
	DOREPLIFETIME(USAttributesComponent, MaxHealth);
	DOREPLIFETIME(USAttributesComponent, Rage);
	DOREPLIFETIME(USAttributesComponent,MaxRage);

	//Optimization
	//DOREPLIFETIME_CONDITION(USAttributesComponent, MaxHealth, COND_InitialOnly);
}


