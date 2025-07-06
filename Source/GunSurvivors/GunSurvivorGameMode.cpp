// Fill out your copyright notice in the Description page of Project Settings.


#include "GunSurvivorGameMode.h"

AGunSurvivorGameMode::AGunSurvivorGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGunSurvivorGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetScore(0);
}

void AGunSurvivorGameMode::SetScore(int NewScore)
{
	// Ensure the new score is non-negative before setting it
	Score = NewScore >= 0 ? NewScore : Score;
}

void AGunSurvivorGameMode::AddScore(int AmountToAdd)
{
	int NewScore = Score + AmountToAdd;
	SetScore(NewScore);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Score: %d"), Score));
}
