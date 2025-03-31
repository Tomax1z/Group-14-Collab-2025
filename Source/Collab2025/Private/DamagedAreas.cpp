// Fill out your copyright notice in the Description page of Project Settings.


#include "DamagedAreas.h"


// Sets default values
ADamagedAreas::ADamagedAreas()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADamagedAreas::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADamagedAreas::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

