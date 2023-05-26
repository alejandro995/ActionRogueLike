// Fill out your copyright notice in the Description page of Project Settings.


#include "SBasePickPowerUp.h"

// Sets default values
ASBasePickPowerUp::ASBasePickPowerUp()
{
 	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASBasePickPowerUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASBasePickPowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASBasePickPowerUp::Pickup()
{
}

void ASBasePickPowerUp::Respawn()
{
}

void ASBasePickPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
}

