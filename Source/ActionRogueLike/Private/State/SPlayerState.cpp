// Fill out your copyright notice in the Description page of Project Settings.


#include "State/SPlayerState.h"

#include "Net/UnrealNetwork.h"


void ASPlayerState::AddCredits(int32 Delta)
{
	if(!ensure(Delta > 0))
	{
		return;
	}

	Credits += Delta;

	if(Delta != 0)
	{
		MulticastCreditsChange(this, Credits, Delta);
	}
}

bool ASPlayerState::RemoveCredits(int32 Delta)
{
	if(!ensure(Delta > 0))
	{
		return false;
	}
	if(Credits < Delta)
	{
		return false;
	}

	Credits -= Delta;

	if (Delta != 0)
	{
		MulticastCreditsChange(this, Credits, -Delta);
	}

	return true;
}

void ASPlayerState::MulticastCreditsChange_Implementation(ASPlayerState* PlayerState, int32 NewCredits, int32 Delta)
{
	OnCreditsChange.Broadcast(PlayerState, NewCredits, Delta);
}


void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
}
