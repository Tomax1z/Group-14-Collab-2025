// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyCell.h"


// Sets default values
AEnergyCell::AEnergyCell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = _Root;

	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_Mesh->SetupAttachment(_Root);

	_Collision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collision"));
	_Collision->SetupAttachment(_Mesh);
}

// Called when the game starts or when spawned
void AEnergyCell::BeginPlay()
{
	Super::BeginPlay();
	
}

FName AEnergyCell::GetConsumableType_Implementation()
{
	return "EnergyCell";
}

void AEnergyCell::OnPickUp_Implementation(AThePlayerCharacter* Picker)
{
	if (Picker)
	{
		Destroy();
	}
}
