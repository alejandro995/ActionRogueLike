// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction.h"

#include "ActionRogueLike/ActionRogueLike.h"
#include "Actions/SActionComponent.h"
#include "Net/UnrealNetwork.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Warning, TEXT("Runninng %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
	
	USActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGamePlayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
	
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Stopped %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::Red);

	//ensureAlways(RepData.bIsRunning);
	
	USActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGamePlayTags.RemoveTags(BlockedTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
}

UWorld* USAction::GetWorld() const
{
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return  Comp->GetWorld();
	}
	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}



bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (isRunning())
	{
		return false;
	}

	USActionComponent* Comp = GetOwningComponent();
	

	if (Comp-> ActiveGamePlayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}


void USAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

bool USAction::isRunning() const
{
	return RepData.bIsRunning;
}

void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
}
