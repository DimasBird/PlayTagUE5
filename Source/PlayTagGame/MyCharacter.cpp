// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"


// Sets default values
AMyCharacter::AMyCharacter() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Posture = 1;
	FlashlightOn = false;

}


const float& AMyCharacter::GetStamina()
{
	return CurrentStamina;
}

void AMyCharacter::ChangeCamera()
{
}

void AMyCharacter::CrouchFunc()
{
}

void AMyCharacter::ChangeFlashlight()
{
	FlashlightOn = not FlashlightOn;
}


void AMyCharacter::Run()
{
}

void AMyCharacter::StopRun()
{
}

void AMyCharacter::StopRunSelf()
{
}

void AMyCharacter::DecreaseStamina()
{
}

void AMyCharacter::IncreaseStamina()
{
}

void AMyCharacter::Interact()
{
}

void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
