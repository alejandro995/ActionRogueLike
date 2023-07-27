// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Effects/MySActionEffect_Thorns.h"

#include "SAttributesComponent.h"
#include "Actions/SActionComponent.h"
#include "FunctionLibrary/SGamePlayFunctionLibrary.h"


UMySActionEffect_Thorns::UMySActionEffect_Thorns()
{
	ReflectFraction = 0.2f;
	Duration = 0.0f;
	Period = 0.0f;
}

void UMySActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwingComp,
	float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	if(Delta <0.0f && OwningActor != InstigatorActor)
	{
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);

		if(ReflectFraction == 0)
		{
			return;
		}

		ReflectFraction = FMath::Abs(ReflectFraction);

		USGamePlayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
		
		
	}
}

void UMySActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	USAttributesComponent* Attibutes = USAttributesComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attibutes)
	{
		Attibutes->OnHealthChanged.AddDynamic(this, &UMySActionEffect_Thorns::OnHealthChanged);
	}
}

void UMySActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	USAttributesComponent* Attibutes = USAttributesComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attibutes)
	{
		Attibutes->OnHealthChanged.RemoveDynamic(this, &UMySActionEffect_Thorns::OnHealthChanged);
	}
}





