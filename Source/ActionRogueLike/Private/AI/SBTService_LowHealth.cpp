// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_LowHealth.h"

#include "AIController.h"
#include "SAttributesComponent.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_LowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//Check the low Health of the minion

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

	if(ensure(BBComp))
	{
		AActor* SelfActor  = Cast<AActor>(BBComp->GetValueAsObject("SelfActor"));

		if(SelfActor)
		{
			AAIController* AIController = OwnerComp.GetAIOwner();

			if (ensure(AIController))
			{
				APawn* AIPAwn = AIController->GetPawn();

				if (ensure(AIPAwn))
				{
					USAttributesComponent* AttributeComp = USAttributesComponent::GetAttributes(AIPAwn);

					bool bIsLowHealth = AttributeComp->isLowHealth();

					if(bIsLowHealth)
					{
						UE_LOG(LogTemp, Warning, TEXT("Loging lowHealth"));
					}

					BBComp->SetValueAsBool(LowHealthKey.SelectedKeyName,bIsLowHealth);
				}
			}
		}
	}
}
