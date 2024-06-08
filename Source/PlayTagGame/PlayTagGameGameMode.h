// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayTagGameGameMode.generated.h"

/**
 * 
 */
UCLASS()
/** \brief GameMode class.
* 
* This class is used to set ANightGuard as the default class in a game.
*/
class PLAYTAGGAME_API APlayTagGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	APlayTagGameGameMode();			/// Constructor for a GameMode.
	
};
