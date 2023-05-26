// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "DrawDebugHelpers.h"
#include "SAttributesComponent.h"
#include "SInteractionComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	HitFlashVFX = CreateDefaultSubobject<UParticleSystem>("HitFlashVFX");
	
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributesComp = CreateDefaultSubobject<USAttributesComponent>("AttributesComp");
	DebugMode = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
	
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributesComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(DebugMode)
	{
		DebugDirectionArrow();
	}
}

void ASCharacter::DebugDirectionArrow()
{
	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);
	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
	
}


// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jumping", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this , &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	

}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	// X = Forward (Red)
	// Y = Right (Green)
	// Z = Up (Blue)

	const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);
	PlayHitFlashEffect();

	
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ASCharacter::SecondaryAttack()
{
	PlayAnimMontage(AttackAnim);
	PlayHitFlashEffect();

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::SecondaryAttack_TimeElapsed, 0.2f);
	
}

void ASCharacter::Dash()
{
	PlayAnimMontage(AttackAnim);
	PlayHitFlashEffect();

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::Dash_TimeElapsed, 0.2f);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
		Attack_TimeElapsed(ProjectileClass);
}

void ASCharacter::SecondaryAttack_TimeElapsed()
{
	Attack_TimeElapsed(SecondaryProjectileClass);
}

void ASCharacter::Dash_TimeElapsed()
{
	Attack_TimeElapsed(DashProjectileClass);
}

void ASCharacter::Attack_TimeElapsed(TSubclassOf<AActor> ProjectileClassArg)
{
	if(ensure(ProjectileClassArg))
	{
		APlayerCameraManager *camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		FVector camStartLocation = camManager->GetCameraLocation();
		FVector camEndLocation  = camStartLocation + ( camManager->GetCameraRotation().Vector() * 10000.0f);
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		float Radius = 5.0f;
		FHitResult Hit;
		FCollisionShape Shape = FCollisionShape::MakeSphere(Radius);
		FCollisionObjectQueryParams objParams;
		objParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		objParams.AddObjectTypesToQuery(ECC_WorldStatic);
		objParams.AddObjectTypesToQuery(ECC_Pawn);

		// SweepSingleByObjectType validates over object type where it was hit
		bool bHit = GetWorld()->SweepSingleByObjectType(Hit, camStartLocation, camEndLocation,
																  FQuat::Identity, objParams, Shape);
		FRotator finalRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, bHit ? Hit.Location : Hit.TraceEnd);
		FTransform SpawnTM = FTransform(finalRotation,HandLocation);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

	
		GetWorld()->SpawnActor<AActor>(ProjectileClassArg , SpawnTM, SpawnParams);

		if(DebugMode)
		{
			DrawDebugLine(GetWorld(), camStartLocation, camEndLocation, FColor::Purple, false, 2.0f, 0, 2.0f );
			DrawDebugSphere(GetWorld(), Hit.Location, 5, 16, FColor::Orange, false, 2.0f);
		}
	}
	
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta <0.0f)
		
	{
		
		USkeletalMeshComponent* Charactermesh = GetMesh();
		UE_LOG(LogTemp, Warning, TEXT("MESH: %s"), *GetNameSafe(Charactermesh));
		Charactermesh->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);

	}

	
	if(NewHealth <=0 && Delta <0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}



void ASCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

void ASCharacter::PlayHitFlashEffect()
{
	UGameplayStatics::SpawnEmitterAttached(HitFlashVFX, GetMesh(), "Muzzle_01");
}


