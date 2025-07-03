// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownCharacter.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATopDownCharacter::ATopDownCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComp);

	CharacterFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterFlipbook"));
	CharacterFlipbook->SetupAttachment(RootComponent);

	GunParent = CreateDefaultSubobject<USceneComponent>(TEXT("GunParent"));
	GunParent->SetupAttachment(RootComponent);

	GunSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("GunSprite"));
	GunSprite->SetupAttachment(GunParent);

	BulletSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPosition"));
	BulletSpawnPosition->SetupAttachment(GunSprite);
}

// Called when the game starts or when spawned
void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (IsValid(PlayerController))
	{
		PlayerController->SetShowMouseCursor(true);
		
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (IsValid(Subsystem))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
}

// Called every frame
void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move the character based on the movement direction and speed
	if (CanMove)
	{
		if (MovementDirection.Length() > 0)
		{
			if (MovementDirection.Length() > 1)
			{
				MovementDirection.Normalize();
			}

			FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;

			FVector CurrentLocation = GetActorLocation();
			// Check if the new location is within the horizontal and vertical bounds
			// If the new location is out of bounds, set it to zero
			float NewXLocation = IsInMapBoundsHorizontal(DistanceToMove.X + CurrentLocation.X) ? DistanceToMove.X : 0.0f;
			float NewZLocation = IsInMapBoundsVertical(DistanceToMove.Y + CurrentLocation.Z) ? DistanceToMove.Y : 0.0f;

			// Calculate the new location based on the current location and the distance to move
			FVector NewLocation = CurrentLocation + FVector(NewXLocation, 0.0f, NewZLocation);

			// Set the new location of the character
			SetActorLocation(NewLocation);
		}
	}

	// Rotate the gun to face the mouse cursor
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (IsValid(PlayerController))
	{
		// Get the mouse position in the world
		FVector MouseWorldLocation, MouseWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		// Get the current location of the character
		FVector CurrentLocation = GetActorLocation();
		// Create a start and target vector for the gun rotation
		FVector Start = FVector(CurrentLocation.X, 0.0f, CurrentLocation.Z);
		FVector Target = FVector(MouseWorldLocation.X, 0.0f, MouseWorldLocation.Z);
		// Calculate the rotation needed to look at the mouse position
		FRotator GunParentRotator = UKismetMathLibrary::FindLookAtRotation(Start, Target);

		GunParent->SetRelativeRotation(GunParentRotator);
	}
}

// Called to bind functionality to input
void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (IsValid(EnhancedInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::MoveTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATopDownCharacter::MoveCompleted);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ATopDownCharacter::MoveCompleted);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ATopDownCharacter::Shoot);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::Shoot);
	}
}

void ATopDownCharacter::MoveTriggered(const FInputActionValue& Value)
{
	FVector2D MoveActionValue = Value.Get<FVector2D>();

	if (CanMove)
	{
		MovementDirection = MoveActionValue;
		CharacterFlipbook->SetFlipbook(RunFlipbook);
		
		FVector FlipbookScale = CharacterFlipbook->GetComponentScale();
		if (MovementDirection.X < 0.0f)
		{
			if (FlipbookScale.X > 0.0f)
				CharacterFlipbook->SetWorldScale3D(FVector(-1, 1, 1));
		}
		else if (MovementDirection.X > 0.0f)
		{
			if (FlipbookScale.X < 0.0f)
				CharacterFlipbook->SetWorldScale3D(FVector(1, -1, 1));
		}
	}
}

void ATopDownCharacter::MoveCompleted(const FInputActionValue& Value)
{
	MovementDirection = FVector2D::ZeroVector;
	CharacterFlipbook->SetFlipbook(IdleFlipbook);
}

void ATopDownCharacter::Shoot(const FInputActionValue& Value)
{

	if (!CanShoot) return;

	CanShoot = false;

	// Bullet spawning logic
	if (!IsValid(BulletClassToSpawn))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BulletClassToSpawn is not set!"));
		return;
	}
	// Spawn the bullet at the bullet spawn position
	ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClassToSpawn, BulletSpawnPosition->GetComponentLocation(), FRotator::ZeroRotator);

	// Get the gun's direction and launch the bullet
	FVector GunDirection = GunParent->GetComponentRotation().Vector();
	Bullet->Launch(FVector2D(GunDirection.X, GunDirection.Z), Bullet->MovementSpeed);

	// Set the shoot cooldown timer
	GetWorldTimerManager().SetTimer(ShootCooldownTimer, this, &ATopDownCharacter::OnShootCooldownTimeout, 1, false, ShootCooldownDuration);
}

bool ATopDownCharacter::IsInMapBoundsHorizontal(float XPos) const
{	
	return (XPos > HorizontalLimits.X && XPos < HorizontalLimits.Y);
}

bool ATopDownCharacter::IsInMapBoundsVertical(float ZPos) const
{
	return (ZPos > VerticalLimits.X && ZPos < VerticalLimits.Y);
}

void ATopDownCharacter::OnShootCooldownTimeout()
{
	CanShoot = true;
}

