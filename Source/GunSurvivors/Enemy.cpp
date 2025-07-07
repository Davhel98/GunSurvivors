// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	SetRootComponent(CapsuleComponent);

	EnemyFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>("EnemyFlipbook");
	EnemyFlipbook->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAlive && CanFollow && IsValid(PlayerRef))
	{
		// Move towards the player
		FVector CurrentLocation = GetActorLocation();
		FVector PlayerLocation = PlayerRef->GetActorLocation();

		FVector DirectionToPlayer = PlayerLocation - CurrentLocation;
		float DistanceToPlayer = DirectionToPlayer.Length();

		if (DistanceToPlayer > StopDistance)
		{
			DirectionToPlayer.Normalize();

			FVector NewLocation = CurrentLocation + (DirectionToPlayer * MovementSpeed * DeltaTime);
			SetActorLocation(NewLocation);
		}

		// Face the player
		CurrentLocation = GetActorLocation();
		float FlipbookXScale = EnemyFlipbook->GetComponentScale().X;

		if (PlayerLocation.X > CurrentLocation.X)
		{
			if (FlipbookXScale < 0.0f)
				EnemyFlipbook->SetWorldScale3D(FVector(1, 1, 1));
		}
		else
		{
			if (FlipbookXScale > 0.0f)
				EnemyFlipbook->SetWorldScale3D(FVector(-1, 1, 1));
		}
	}
}

void AEnemy::Die()
{
	if (!IsAlive) return;

	IsAlive = false;
	CanFollow = false;

	EnemyFlipbook->SetFlipbook(DeadFlipbook);
	EnemyFlipbook->SetTranslucentSortPriority(-5);

	EnemyDiedDelegate.Broadcast();

	UGameplayStatics::PlaySound2D(GetWorld(), DieSound);

	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AEnemy::OnDestroyTimerTimeout, 1.0f, false, 10.0f);
}

void AEnemy::OnDestroyTimerTimeout()
{
	Destroy(); // Destroy the enemy actor after a delay
}

