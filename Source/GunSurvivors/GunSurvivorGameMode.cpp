// Fill out your copyright notice in the Description page of Project Settings.


#include "GunSurvivorGameMode.h"

#include "Kismet/GameplayStatics.h"

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
	ScoreChangedDelegate.Broadcast(Score);
}

void AGunSurvivorGameMode::AddScore(int AmountToAdd)
{
	int NewScore = Score + AmountToAdd;
	SetScore(NewScore);
}

void AGunSurvivorGameMode::RestartGame()
{
	GetWorldTimerManager().SetTimer(RestartGameTimer, this, &AGunSurvivorGameMode::OnRestartGameTimerTimeout, 1, false, TimeBeforeRestart);
}

void AGunSurvivorGameMode::OnRestartGameTimerTimeout()
{
	FString CurrentLevelName = GetWorld()->GetMapName();
	UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevelName));
}
