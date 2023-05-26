// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributesComp = CreateDefaultSubobject<USAttributesComponent>("AttributeComp");
	//Trigger when health is change(Damage/Health)
	AttributesComp->OnHealthChanged.AddDynamic(this,&ASTargetDummy::OnHealthChanged);
}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	if(Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
	
}




