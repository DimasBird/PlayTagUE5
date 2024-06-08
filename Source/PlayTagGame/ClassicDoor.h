// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"
#include "ClassicDoor.generated.h"

UCLASS()
/** \brief The Door class.
* 
* \warning This class is made just to test the Interaction method.
*/
class PLAYTAGGAME_API AClassicDoor : public AActor
{
	GENERATED_BODY()
	
public:
	AClassicDoor();

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* Sound1;
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* Sound2;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Door;
	UPROPERTY(VisibleAnywhere, Category = "Box")
		UBoxComponent* BoxComponent;

	bool IsClosed = true;
	UPROPERTY(BlueprintReadOnly)
		float degree = 90;
	UPROPERTY(BlueprintReadWrite, Category = "Reversed")
		bool inversed = false;
	UPROPERTY(EditAnywhere, Category="MaxDoorOpenDegree")
		float MaxDegree = 110;

	UPROPERTY(EditAnywhere)
		UCurveFloat* Curve;
		FTimeline Timeline;
		AMyCharacter* Actor = nullptr;
		bool thatSide;


	UFUNCTION()
		void Interaction();
		void DoorOpen(float value);
		void SetOnSameSide();
		virtual void Tick(float Time) override;
};
