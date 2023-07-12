// Fill out your copyright notice in the Description page of Project Settings.


#include "State/SPlayerState.h"


void ASPlayerState::AddCredits(int32 Delta)
{
	if(ensure(Delta > 0.0f))
	{
		return;
	}

	Credits += Delta;
	OnCreditsChange.Broadcast(this, Credits, Delta);
}

bool ASPlayerState::RemoveCredits(int32 Delta)
{
	if(!ensure(Delta > 0.0f))
	{
		return false;
	}
	if(Credits < Delta)
	{
		return false;
	}

	Credits -= Delta;
	OnCreditsChange.Broadcast(this, Credits, -Delta);

	return true;
}
