// Fill out your copyright notice in the Description page of Project Settings.


#include "State/SPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "SaveGame/SSaveGame.h"


void ASPlayerState::AddCredits(int32 Delta)
{
	if(!ensure(Delta > 0))
	{
		return;
	}

	Credits += Delta;

	if(Delta != 0)
	{
		OnCreditsChange.Broadcast(this, Credits, Delta);
		//MulticastCreditsChange(this, Credits, Delta);
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
		OnCreditsChange.Broadcast(this, Credits, -Delta);
		//MulticastCreditsChange(this, Credits, -Delta);
	}

	return true;
}

void ASPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChange.Broadcast(this, Credits, Credits - OldCredits);
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		//The State has been set up early in the gamemode. 
		//Credits = SaveObject->Credits;
		AddCredits(SaveObject->Credits);
	}
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}
/* This is used when multicast method is been used
void ASPlayerState::MulticastCreditsChange_Implementation(ASPlayerState* PlayerState, int32 NewCredits, int32 Delta)
{
	OnCreditsChange.Broadcast(PlayerState, NewCredits, Delta);
}
*/

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}


void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
}
