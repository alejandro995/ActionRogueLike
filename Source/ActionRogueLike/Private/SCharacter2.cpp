// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter2.h"

// Sets default values
ASCharacter2::ASCharacter2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASCharacter2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

