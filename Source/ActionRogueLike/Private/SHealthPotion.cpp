// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributesComponent.h"
#include "State/SPlayerState.h"

ASHealthPotion::ASHealthPotion()
{
	MeshComponent->Activate();
	MeshComponent->SetVisibility(true);
	activated = true;

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
				if(!AtrributeComp->isFullHealth() & activated & bHasEnoughCredits)
				{
					MeshComponent->Deactivate();
					AtrributeComp->ApplyHealthChange(this,+25.0f);
					MeshComponent->SetVisibility(false);
					activated = false;
				}

				GetWorldTimerManager().SetTimer(TimerHandle_PickUpActivation, this, &ASHealthPotion::Activation_TimeElapsed, 10.0f);
			}
		}
	}
}

void ASHealthPotion::Activation_TimeElapsed()
{
	MeshComponent->Activate();
	MeshComponent->SetVisibility(true);
	activated = true;
	
}




