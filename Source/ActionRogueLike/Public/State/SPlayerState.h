// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChange, ASPlayerState*, PlayerState, int32, NewCredits, int32, Delta);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category= "PlayerState|Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category="Playerstate|Credits")
	bool RemoveCredits(int32 Delta);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastCreditsChange(ASPlayerState* PlayerState, int32 NewCredits, int32 Delta);

	UPROPERTY(BlueprintAssignable,  Category= "Events")
	FOnCreditsChange OnCreditsChange;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="PlayerState|Credits")
	int32 Credits;
};
