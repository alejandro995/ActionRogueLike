// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGamePlayInterface.h"
#include "GameFramework/Actor.h"
#include "SBasePickPowerUp.generated.h"

class UStaticMeshComponent;

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASBasePickPowerUp : public AActor, public ISGamePlayInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	UStaticMeshComponent* MeshComponent;
	// Sets default values for this actor's properties
	
	void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetIteractText_Implementation(APawn* InstigatorPawn);
	
	ASBasePickPowerUp();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Pickup();

	virtual void Respawn();

};
