// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasePickPowerUp.h"
#include "SCoin.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASCoin : public ASBasePickPowerUp
{
	GENERATED_BODY()

protected:
	bool activated;

	FTimerHandle TimeHanlde_PickupActivation;

	void Interact_Implementation(APawn* InstigatorPawn);

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 CreditsAmount;
	

public:
	void Activation_TimeElapsed();
	
	ASCoin();
	
};
