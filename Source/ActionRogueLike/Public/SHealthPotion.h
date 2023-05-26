// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasePickPowerUp.h"
#include "SHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASBasePickPowerUp
{
	GENERATED_BODY()

protected:
	bool activated;
	
	FTimerHandle TimerHandle_PickUpActivation;

	void Interact_Implementation(APawn* InstigatorPawn);

public:
	void Activation_TimeElapsed();

	ASHealthPotion();
};
