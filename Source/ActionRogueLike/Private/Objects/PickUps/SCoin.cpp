// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/PickUps/SCoin.h"
#include "State/SPlayerState.h"

ASCoin::ASCoin()
{
	MeshComponent->Activate();
	MeshComponent->SetVisibility(true);
	activated = true;

	CreditsAmount = 80;
}



void ASCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	if (!ensure(InstigatorPawn))
	{
		return;
	}
	
	if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
	{
			PS->AddCredits(CreditsAmount);

			MeshComponent->Deactivate();
			MeshComponent->SetVisibility(false);
			activated = false;
	}
	

	GetWorldTimerManager().SetTimer(TimeHanlde_PickupActivation, this, &ASCoin::Activation_TimeElapsed, 10.0f);
}

void ASCoin::Activation_TimeElapsed()
{
	MeshComponent->Activate();
	MeshComponent->SetVisibility(true);
	activated = true;
}

