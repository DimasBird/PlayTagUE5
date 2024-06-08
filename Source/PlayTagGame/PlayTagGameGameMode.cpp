// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayTagGameGameMode.h"

APlayTagGameGameMode::APlayTagGameGameMode() : Super()
{
	ConstructorHelpers::FClassFinder<APawn>ActorNightGuard(TEXT("/Game/characters/NightGuard/BP_MyNightGuard"));

	DefaultPawnClass = ActorNightGuard.Class;

};