// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributesComponent.generated.h"
// Declare a comomn multicast for both 2 delegates OnHealthChange and OnRageChange
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAtributeChanged, AActor*, InstigatorActor,  USAttributesComponent*, OwningComp,  float, NewValue, float, Delta);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Atributes")
	static USAttributesComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category="Atributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="Atributes", meta = (DisplayName = "IsLowHealth"))
	static bool IsActorLowHealth(AActor* Actor);
	
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Atributtes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Atributtes")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Atributtes")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Atributtes")
	float MaxRage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Atributtes")
	float LowHealthTreshold;
	
	// HealthMax, Stamina, Strength

	UFUNCTION(NetMulticast, Reliable) // FIXME : mark as unrealiable 
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);

public:

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable)
	bool isAlive() const;

	UFUNCTION(BlueprintCallable)
	bool isFullHealth() const;

	UFUNCTION(BlueprintCallable)
	bool isLowHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	float getRage() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnAtributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAtributeChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category= "Attributes")
	bool ApplyRage(AActor* InstigatorActor, float Delta);
	
};
