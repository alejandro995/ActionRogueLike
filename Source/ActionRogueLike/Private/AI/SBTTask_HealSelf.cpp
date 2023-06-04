// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealSelf.h"

#include "AIController.h"
#include "SAttributesComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AAIController* AIController = OwnerComp.GetAIOwner();

	if(ensure(AIController))
	{
		ACharacter* AIPawn = Cast<ACharacter>(AIController->GetPawn());

		if(AIPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		AActor* SelfActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));

		if(USAttributesComponent::IsActorLowHealth(SelfActor) == true)
		{

			USAttributesComponent* AttributesComponent = USAttributesComponent::GetAttributes(SelfActor);

			if (ensure(AttributesComponent))
			{
				AttributesComponent->ApplyHealthChange(SelfActor, 100);
			}
			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Failed;
}
