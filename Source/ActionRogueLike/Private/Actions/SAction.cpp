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

	bIsRunning = true;
	
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Warning, TEXT("Stopped %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::Red);

	ensureAlways(bIsRunning);
	
	USActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGamePlayTags.RemoveTags(BlockedTags);

	bIsRunning = false;
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


void USAction::OnRep_IsRunning()
{
	if (bIsRunning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}

bool USAction::isRunning() const
{
	return bIsRunning;
}

void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, bIsRunning);
}
