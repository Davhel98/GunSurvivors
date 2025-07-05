// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Enemy.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(SphereComponent);

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite");
	SpriteComponent->SetupAttachment(RootComponent);

	MovementDirection = FVector2D(1, 0);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OverlapBegin);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLaunched)
	{
		// Calculate the new location based on the movement direction and speed
		FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = CurrentLocation + FVector(DistanceToMove.X, 0.0f, DistanceToMove.Y);

		SetActorLocation(NewLocation);
	}
}

void ABullet::Launch(FVector2D Direction, float Speed)
{
	if (IsLaunched) return;

	IsLaunched = true;

	MovementDirection = Direction;
	MovementSpeed = Speed;

	GetWorldTimerManager().SetTimer(DeleteTimer, this, &ABullet::OnDeleteTimerTimeout, 1.0f, false, DeleteTime);
}

void ABullet::OnDeleteTimerTimeout()
{
	Destroy();
}

void ABullet::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);

	if (IsValid(Enemy) && Enemy->IsAlive)
	{
		// Kill the enemy
		Enemy->Die();

		// Disable the bullet after hitting an enemy
		DisableBullet();
	}
}

void ABullet::DisableBullet()
{
	if (IsDisabled) return;

	IsDisabled = true;

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SpriteComponent->DestroyComponent();
}

