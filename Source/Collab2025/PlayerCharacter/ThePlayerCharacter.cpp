// Fill out your copyright notice in the Description page of Project Settings.


#include "ThePlayerCharacter.h"
#include "IConsumable.h"
#include "IPickUpable.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AThePlayerCharacter::AThePlayerCharacter()
{
	_Collision = GetCapsuleComponent();
	_CharacterMovement = GetCharacterMovement();
	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_Camera->SetupAttachment(_Collision);
	_Camera->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f));
	_Camera->bUsePawnControlRotation = true;

	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	_CharacterMovement->MaxWalkSpeedCrouched = _CrouchSpeed;

	_HoldArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("HoldArrow"));
	_HoldArrow->SetupAttachment(_Camera);
	_HoldArrow->bHiddenInGame = true;
}

void AThePlayerCharacter::IAMove_Implementation(const FInputActionInstance& Instance)
{
	if(Controller != nullptr)
	{
		const FVector2d MoveValue = Instance.GetValue().Get<FVector2d>();

		if(MoveValue.Y != 0.f)
		{
			AddMovementInput(GetActorForwardVector(), MoveValue.Y);
		}
		if(MoveValue.X != 0.f)
		{
			AddMovementInput(GetActorRightVector(), MoveValue.X);
		}
	}
}

void AThePlayerCharacter::IALook_Implementation(const FInputActionInstance& Instance)
{
	if(Controller != nullptr)
	{
		const FVector2d AxisValue = Instance.GetValue().Get<FVector2d>();

		if(AxisValue.Y != 0.f)
		{
			AddControllerPitchInput(AxisValue.Y);
		}
		if(AxisValue.X != 0.f)
		{
			AddControllerYawInput(AxisValue.X);
		}
	}
}

void AThePlayerCharacter::IAInteract_Implementation(const FInputActionInstance& Instance)
{
	IIAInterface::IAInteract_Implementation(Instance);
}

void AThePlayerCharacter::IASprint_Implementation(const FInputActionInstance& Instance)
{
	_CharacterMovement->MaxWalkSpeed = _SprintSpeed;
}

void AThePlayerCharacter::IAStopSprinting_Implementation(const FInputActionInstance& Instance)
{
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
}

void AThePlayerCharacter::IACrouch_Implementation(const FInputActionInstance& Instance)
{
	Crouch();
}

void AThePlayerCharacter::IAStand_Implementation(const FInputActionInstance& Instance)
{
	UnCrouch();
}

void AThePlayerCharacter::IAJump_Implementation(const FInputActionInstance& Instance)
{
	Jump();
}

void AThePlayerCharacter::SetIsSprinting(bool bSprinting)
{
	bIsSprinting = bSprinting;
}

void AThePlayerCharacter::SetSpeedRatio_Implementation(float SprintSpeed, float BaseSpeed)
{
	_SprintBaseSpeedRatio = SprintSpeed / BaseSpeed;
}

// Called when the game starts or when spawned
void AThePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AThePlayerCharacter::PickupObject(AActor* ObjectToPickup)
{
	if (!ObjectToPickup || bIsHoldingObject)
	{
		return;
	}

	if (!ObjectToPickup->GetClass()->ImplementsInterface(UIPickUpable::StaticClass()))
	{
		return;
	}

	bIsHoldingObject = true;

	if (ObjectToPickup->GetClass()->ImplementsInterface(UIConsumable::StaticClass()))
	{
		const FName ConsumableType = IIConsumable::Execute_GetConsumableType(ObjectToPickup);

		if (ConsumableType == TEXT("OxygenTank"))
		{
			UE_LOG(LogTemp, Warning, TEXT("Oxygen"));
			_NumOfOxygenTanks += 1;
		}
		else if (ConsumableType == TEXT("EnergyCell"))
		{
			UE_LOG(LogTemp, Warning, TEXT("energy"));
			_NumOfPowerCells += 1;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Picked up consumable with unexpected type '%s'"), *ConsumableType.ToString());
		}
	}
	if (AActor* ActorToDestroy = Cast<AActor>(ObjectToPickup))
	{
		ActorToDestroy->Destroy();
	}
}


// Called to bind functionality to input
void AThePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

AProtoMech* AThePlayerCharacter::GetProtoMech()
{
	UWorld* W = GetWorld();
	if (!W) return nullptr;

	return Cast<AProtoMech>(
		UGameplayStatics::GetActorOfClass(W, AProtoMech::StaticClass())
	);
}

void AThePlayerCharacter::PlayerSpawnOxygen()
{
	if (AProtoMech* ProtoMech = GetProtoMech())
	{
		ProtoMech->SpawnOxygenPickup();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No ProtoMech found in world"));
	}
}

void AThePlayerCharacter::PlayerSpawnEnergy()
{
	if (AProtoMech* ProtoMech = GetProtoMech())
	{
		ProtoMech->SpawnEnergyPickup();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No ProtoMech found in world"));
	}
}