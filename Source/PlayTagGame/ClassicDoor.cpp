// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicDoor.h"

// Sets default values
AClassicDoor::AClassicDoor() : Super()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->InitBoxExtent(FVector(50, 6.25, 100));
	BoxComponent->SetCollisionProfileName("Trigger");

	Sound1 = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio1"));
	Sound2 = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio2"));

	Curve = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/objects/ClassicDoor/DoorDegreeCurve"));
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	if (Curve) {
		FOnTimelineFloat CurrentTime;
		CurrentTime.BindUFunction(this, FName("DoorInteraction"));
		Timeline.AddInterpFloat(Curve, CurrentTime);
	}
}

void AClassicDoor::Interaction()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Interacted")));
	if (IsClosed) {
		SetOnSameSide();
		Timeline.Play();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Opening")));
		Sound1->Play();
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Closing")));
		Timeline.Reverse();
		Sound2->Play();
	}
	IsClosed = !IsClosed;
}

void AClassicDoor::DoorOpen(float value)
{
	float Angle = thatSide ? -MaxDegree : MaxDegree;
	FRotator Rotator = FRotator(0.f, Angle * value *3, 0.f);
	Door->SetRelativeRotation(Rotator);
}

void AClassicDoor::SetOnSameSide()
{
	if (Actor) {
		FVector ActorFVector = Actor->GetActorForwardVector();
		FVector DoorFV = Door->GetForwardVector();
		thatSide = (FVector::DotProduct(DoorFV, ActorFVector) >= 0);
	}
}

void AClassicDoor::Tick(float Time) {
	Super::Tick(Time);
	Timeline.TickTimeline(Time);
	float val = Timeline.GetPlaybackPosition();
	DoorOpen(val);
}
