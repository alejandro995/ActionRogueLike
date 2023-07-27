// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/SPickPowerUp_Action.h"

#include "Actions/SAction.h"
#include "Actions/SActionComponent.h"

ASPickPowerUp_Action::ASPickPowerUp_Action()
{
	MeshComponent->Activate();
	MeshComponent->SetVisibility(true);
	activated = true;
}

void ASPickPowerUp_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	USActionComponent* ActionComp = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass()));

	if (ActionComp)
	{
		if (ActionComp->GetAction(ActionToGrant))
		{
			FString DebugMsg = FString::Printf(TEXT("Action '%s' aleready know."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,DebugMsg);
			return;
		}

		ActionComp->AddAction(InstigatorPawn, ActionToGrant);
		MeshComponent->Deactivate();
		MeshComponent->SetVisibility(false);
		activated = false;

		GetWorldTimerManager().SetTimer(TimeHanlde_PickupActivation, this, &ASPickPowerUp_Action::Activation_TimeElapsed, 10.0f);
		
	}
}

void ASPickPowerUp_Action::Activation_TimeElapsed()
{
	MeshComponent->Activate();
	MeshComponent->SetVisibility(true);
	activated = true;
}


