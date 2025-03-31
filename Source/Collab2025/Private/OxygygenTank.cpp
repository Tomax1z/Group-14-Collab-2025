// Fill out your copyright notice in the Description page of Project Settings.


#include "OxygygenTank.h"


// Sets default values
AOxygygenTank::AOxygygenTank()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AOxygygenTank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOxygygenTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FName AOxygygenTank::GetConsumableType_Implementation()
{
	return "OxygenTank";
}
