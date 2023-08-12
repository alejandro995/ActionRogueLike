// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributesComponent.h"
#include "State/SPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ASHealthPotion::ASHealthPotion()
{
	MeshComponent->Activate();
	MeshComponent->SetVisibility(true);

	CreditCost = 50;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{

	USAttributesComponent* AtrributeComp = Cast<USAttributesComponent>(InstigatorPawn->GetComponentByClass(USAttributesComponent::StaticClass()));

	bool bHasEnoughCredits = false;

	if(ensure(AtrributeComp) && !AtrributeComp->isFullHealth())
	{
		if (ASPlayerState* PlayerState =  InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			if(PlayerState->RemoveCredits(CreditCost) && AtrributeComp->ApplyHealthChange(this, AtrributeComp->GetMaxHealth()))
			{
					MeshComponent->Deactivate();
					MeshComponent->SetVisibility(false);

				GetWorldTimerManager().SetTimer(TimerHandle_PickUpActivation, this, &ASHealthPotion::Activation_TimeElapsed, 10.0f);
			}
		}
	}
}

FText ASHealthPotion::GetIteractText_Implementation(APawn* InstigatorPawn)
{
	USAttributesComponent* AtrributeComp = Cast<USAttributesComponent>(InstigatorPawn->GetComponentByClass(USAttributesComponent::StaticClass()));

	if (ensure(AtrributeComp) && !AtrributeComp->isFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}


	return  FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximu."), CreditCost);
}

void ASHealthPotion::Activation_TimeElapsed()
{
	MeshComponent->Activate();
	MeshComponent->SetVisibility(true);
}

#undef LOCTEXT_NAMESPACE



