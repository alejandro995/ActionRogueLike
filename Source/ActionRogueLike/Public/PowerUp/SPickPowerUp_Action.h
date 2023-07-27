// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasePickPowerUp.h"
#include "SPickPowerUp_Action.generated.h"

class USAction;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPickPowerUp_Action : public ASBasePickPowerUp
{
	GENERATED_BODY()

protected:
	bool activated;

	FTimerHandle TimeHanlde_PickupActivation;
	
	UPROPERTY(EditAnywhere, Category="Powerup")
	TSubclassOf<USAction> ActionToGrant;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

	void Activation_TimeElapsed();

	ASPickPowerUp_Action();
};
