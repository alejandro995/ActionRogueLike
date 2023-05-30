// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SAttributesComponent.h"
#include "SCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AttributesComp = CreateDefaultSubobject<USAttributesComponent>("AttributesComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributesComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}



void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta < 0.0f)
	{
		if(InstigatorActor != this)
		{
			setTargetActor(InstigatorActor);
			
		}

		if(NewHealth <= 0.0f)
		{
			// Stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if(AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
				
			}

			//Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			

			//Set lifespan
			SetLifeSpan(10.0f);
		}
	}
}

void ASAICharacter::setTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if(AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{

	setTargetActor(Pawn);



		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	
}



