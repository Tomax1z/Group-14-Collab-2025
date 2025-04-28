// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"

#include "ISpeedControllable.h"
#include "Collab2025/PlayerCharacter/ThePlayerCharacter.h"


// Sets default values
ALever::ALever()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = _RootSceneComponent;

	_LeverMeshBase= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverMeshBase"));
	_LeverMeshBase->SetupAttachment(_RootSceneComponent);

	_LeverMesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverMesh"));
	_LeverMesh->SetupAttachment(_LeverMeshBase);
}

// Called when the game starts or when spawned
void ALever::BeginPlay()
{
	Super::BeginPlay();
	_LeverMeshBase->SetRelativeRotation(DefaultRotation);
	NotifyTargetActor();
}

// Called every frame
void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALever::ToggleLever()
{
	_bIsPulled = !_bIsPulled;

	const FRotator NewRot = _bIsPulled ? PulledRotation : DefaultRotation;
	_LeverMeshBase->SetRelativeRotation(NewRot);
	
	UE_LOG(LogTemp, Log, TEXT("Lever toggled. New state (bIsPulled): %s"), _bIsPulled ? TEXT("True") : TEXT("False"));

	NotifyTargetActor();
}

void ALever::NotifyTargetActor()
{
	if (!_TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Lever: _TargetActor is not set!"));
		return;
	}

	if (_TargetActor->GetClass() -> ImplementsInterface(UISpeedControllable::StaticClass()))
	{
		IISpeedControllable::Execute_SetSprintStatus(_TargetActor, _bIsPulled);
		UE_LOG(LogTemp, Log, TEXT("Lever notified %s (via Execute_) to set sprint status to: %s"), *_TargetActor->GetName(), _bIsPulled ? TEXT("True") : TEXT("False"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Lever: _TargetActor '%s' does not implement ISpeedControllable!"), *_TargetActor->GetName());
	}
}

void ALever::Interact_Implementation(AActor* Interactor)
{
	ToggleLever();
	if (AThePlayerCharacter* PlayerChar = Cast<AThePlayerCharacter>(Interactor))
	{
		PlayerChar->SetIsSprinting(_bIsPulled);
	}
}