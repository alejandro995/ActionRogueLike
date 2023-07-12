// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction.h"

#include "Actions/SActionComponent.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Runninng %s"), *GetNameSafe(this));
	USActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGamePlayTags.AppendTags(GrantsTags);

	bIsRunning = true;
	
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Stopped %s"), *GetNameSafe(this));

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


bool USAction::isRunning() const
{
	return bIsRunning;
}