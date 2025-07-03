// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownCharacter.h"

// Sets default values
ATopDownCharacter::ATopDownCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComp);

	CharacterFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterFlipbook"));
	CharacterFlipbook->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (IsValid(PlayerController))
	{
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
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Fire!");
}

bool ATopDownCharacter::IsInMapBoundsHorizontal(float XPos) const
{	
	return (XPos > HorizontalLimits.X && XPos < HorizontalLimits.Y);
}

bool ATopDownCharacter::IsInMapBoundsVertical(float ZPos) const
{
	return (ZPos > VerticalLimits.X && ZPos < VerticalLimits.Y);
}

