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

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (IsValid(GameMode))
	{
		GameModeRef = Cast<AGunSurvivorGameMode>(GameMode);
	}

	if (!IsValid(PlayerRef))
	{
		AActor* ActorRef = UGameplayStatics::GetActorOfClass(GetWorld(), ATopDownCharacter::StaticClass());
		if (IsValid(ActorRef))
		{
			PlayerRef = Cast<ATopDownCharacter>(ActorRef);
			PlayerRef->PlayerDiedDelegate.AddDynamic(this, &AEnemySpawner::OnPlayerDied);
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

	Enemy->EnemyDiedDelegate.AddDynamic(this, &AEnemySpawner::OnEnemyDied);
}

void AEnemySpawner::OnEnemyDied()
{
	int ScoreToAdd = 10; // Define the score to add for each enemy killed
	GameModeRef->AddScore(ScoreToAdd);
}

void AEnemySpawner::OnPlayerDied()
{
	StopSpawning();

	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Enemies);

	for (AActor* EnemyActor : Enemies)
	{
		AEnemy* Enemy = Cast<AEnemy>(EnemyActor);
		if (IsValid(Enemy))
		{
			Enemy->CanFollow = false; // Ensure all enemies stop when the player dies
		}
	}

	GameModeRef->RestartGame();
}

