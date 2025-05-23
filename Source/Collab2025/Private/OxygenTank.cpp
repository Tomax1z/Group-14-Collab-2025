﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "OxygenTank.h"


// Sets default values
AOxygenTank::AOxygenTank()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = _Root;

	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_Mesh->SetupAttachment(_Root);

	_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	_Collision->SetupAttachment(_Mesh);
}

// Called when the game starts or when spawned
void AOxygenTank::BeginPlay()
{
	Super::BeginPlay();
	
}

FName AOxygenTank::GetConsumableType_Implementation()
{
	return "OxygenTank";
}

void AOxygenTank::OnPickUp_Implementation(AThePlayerCharacter* Picker)
{
	
}
