// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor,  USAttributesComponent*, OwningComp,  float, NewHealth, float, Delta);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributesComponent();

protected:
	// EditAnywhere - edit in Bp editor and por-instance in level
	// VisibleAnywhere - 'read-only' in editor and level, (Use for Components)
	// EditDefaultsOnly - hide variable per-instance, edit in BP editor only
	// VisibleDefaultsOnly - 'read-only' access for variables, only in Bp editor (uncommon)
	// EditInstanceOnly - allow only editing of instance (eg. when placed in level)
	// --
	// BlueprintReadOnly - read-only in the Blueprint scripting (does not affect 'details' -panel)
	// BlueprintReadWrite - read-write access in Blueprints
	//--
	// Category = "" - display for details panels and Blueprints context menu.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Atributtes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Atributtes")
	float MaxHealth;

	
	// HealthMax, Stamina, Strength

public:

	UFUNCTION(BlueprintCallable)
	bool isAlive() const;

	UFUNCTION(BlueprintCallable)
	bool isFullHealth() const;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool ApplyHealthChange(float Delta);
};
