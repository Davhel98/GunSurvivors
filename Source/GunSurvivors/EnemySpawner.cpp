// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(PlayerRef))
	{
		AActor* ActorRef = UGameplayStatics::GetActorOfClass(GetWorld(), ATopDownCharacter::StaticClass());
		if (IsValid(ActorRef))
		{
			PlayerRef = Cast<ATopDownCharacter>(ActorRef);
		}
	}
	
	// Start the spawning process on begin play
	StartSpawning();
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::OnSpawnTimerTimeout()
{
	SpawnEnemy();
}

void AEnemySpawner::StartSpawning()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawner::OnSpawnTimerTimeout, SpawnInterval, true, SpawnInterval);
}

void AEnemySpawner::StopSpawning()
{
	GetWorldTimerManager().ClearTimer(SpawnTimer);
}

void AEnemySpawner::SpawnEnemy()
{
	// Spawn the enemy
	FVector2D RandomPosition = FVector2D(FMath::VRand());
	RandomPosition.Normalize();
	RandomPosition *= SpawnDistance;
	
	FVector SpawnLocation = GetActorLocation() + FVector(RandomPosition.X, 0.0f, RandomPosition.Y);
	
	AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyActorToSpawn, SpawnLocation, FRotator::ZeroRotator);
	SetupEnemy(SpawnedEnemy);

	// Increase the difficulty
	TotalEnemyCount++;
	if (TotalEnemyCount % DifficultySpikeInterval == 0)
	{
		SpawnInterval = FMath::Max(SpawnTimeMinimumLimit, SpawnInterval - SpawnTimerDecreaseFactor);

		StopSpawning();
		StartSpawning();
	}
}

void AEnemySpawner::SetupEnemy(AEnemy* Enemy)
{
	if (!IsValid(Enemy)) return;

	Enemy->PlayerRef = PlayerRef;
	Enemy->CanFollow = true;
}

