// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributesComponent.h"
#include "SCharacter.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "State/SPlayerState.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("SU.SpawnBots"), false, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);


ASGameModeBase::ASGameModeBase()
{

	SpawnTimerInterval = 2.0f;
	CreditsPerKill = 20;

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000;

	PlayerStateClass = ASPlayerState::StaticClass();
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	//Continuous timer to spawn is more bots.
	//Actual amount of bots and whether its allowed to spawn determined by spawn logic later in the chain...
	GetWorldTimerManager().SetTimer(TimeHandle_SpawnBots, this,&ASGameModeBase::SpawnBotTimeElapsed, SpawnTimerInterval, true);

	if(ensure(PowerUpClasses.Num() > 0))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, PowerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerSpawnQueryCompleted);
		}
	}
	
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributesComponent* AttributeComp = USAttributesComponent::GetAttributes(Bot);

		if (ensure(AttributeComp) && AttributeComp->isAlive())
		{
			AttributeComp->Kill(this); //Pass in player? for kill credit
		}
	}
}


void ASGameModeBase::SpawnBotTimeElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot Spawning disabled via cvar 'CVarSpawnBots '"))
		return;
	}
	
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributesComponent* AttributeComp = USAttributesComponent::GetAttributes(Bot);

		if (ensure(AttributeComp) && AttributeComp->isAlive())
		{
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots"), NrOfAliveBots);

	float MaxBotCount = 10.0f;

	if(DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
		
	}
	if(NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("AT maximun bot capacity, Skipping bot spawn."));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if(ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnBotSpawnQueryCompleted);
	}

}

void ASGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{

	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if(Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		//Track all tge used Spawn Location
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Emerald, false, 100.0f);
	}
}

void ASGameModeBase::OnPowerSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstace, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed"));
		return;
	}

	TArray<FVector> Locations = QueryInstace->GetResultsAsLocations();
	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;

	while (SpawnCounter <DesiredPowerupCount && Locations.Num() >0)
	{
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num()-1);
		FVector PickedLocation = Locations[RandomLocationIndex];

		Locations.RemoveAt(RandomLocationIndex);

		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPowerupDistance)
			{
				bValidLocation = false;
				break;
			}
		}

		if (!bValidLocation)
		{
			continue;
		}

		int32 RandomClassIndex = FMath::RandRange(0, PowerUpClasses.Num() - 1 );
		TSubclassOf<AActor> RandomPowerupClass = PowerUpClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);

		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}


void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
	
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_ResponseDelay;
		FTimerDelegate Delegate;

		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_ResponseDelay, Delegate, RespawnDelay, false);
	}
	
	
	APawn* KillerPawn = Cast<APawn>(Killer);
	{
		if (KillerPawn)
		{
			if (ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>())
			{
				GEngine->AddOnScreenDebugMessage(-1,1.0f, FColor::Red, "Credits Applied After minion Killed");
				PS->AddCredits(CreditsPerKill);
			}
			
		}
	}
}


void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}



