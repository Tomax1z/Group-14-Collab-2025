// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyCell.h"


// Sets default values
AEnergyCell::AEnergyCell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnergyCell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnergyCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FName AEnergyCell::GetConsumableType_Implementation()
{
	return "EnergyCell";
}

