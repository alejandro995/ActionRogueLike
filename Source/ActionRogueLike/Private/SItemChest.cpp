// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshComp");
	RootComponent = BaseMeshComp;
	
	LidMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("LidMeshComp");
	LidMeshComp->SetupAttachment(BaseMeshComp);

	TargetPitch = 110;

	SetReplicates(true);
	
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	
	bLidOpened = !bLidOpened;
	OnRep_LidOpened();
}

void ASItemChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMeshComp->SetRelativeRotation(FRotator(CurrPitch,0,0));
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASItemChest, bLidOpened);
	
}

