// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TimerHandle.h"

#include "Enemy.h"

#include "EnemySpawner.generated.h"

UCLASS()
class GUNSURVIVORS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemy> EnemyActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnDistance = 500.0f;

	FTimerHandle SpawnTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int TotalEnemyCount = 0;
	UPROPERTY(EditAnywhere)
	int DifficultySpikeInterval = 10;
	UPROPERTY(EditAnywhere)
	float SpawnTimeMinimumLimit = 0.5f;
	UPROPERTY(EditAnywhere)
	float SpawnTimerDecreaseFactor = 0.05f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnSpawnTimerTimeout();
	void StartSpawning();
	void StopSpawning();
	void SpawnEnemy();

};
