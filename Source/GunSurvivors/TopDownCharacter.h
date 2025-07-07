// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Engine/TimerHandle.h"
#include "Bullet.h"
#include "GameFramework/Controller.h"
#include "Sound/SoundBase.h"

#include "TopDownCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDiedDelegate);
UCLASS()
class GUNSURVIVORS_API ATopDownCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATopDownCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* CharacterFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gun")
	USceneComponent* GunParent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gun")
	UPaperSpriteComponent* GunSprite;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gun")
	USceneComponent* BulletSpawnPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flipbook")
	UPaperFlipbook* IdleFlipbook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flipbook")
	UPaperFlipbook* RunFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Bounds")
	FVector2D HorizontalLimits;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Bounds")
	FVector2D VerticalLimits;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float MovementSpeed = 200.0f;
	UPROPERTY(BlueprintReadWrite, Category="Movement")
	FVector2D MovementDirection;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Movement")
	bool CanMove = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shooting")
	bool CanShoot = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shooting")
	float ShootCooldownDuration = 0.3f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shooting")
	TSubclassOf<ABullet> BulletClassToSpawn;
	FTimerHandle ShootCooldownTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAlive = true;

	FPlayerDiedDelegate PlayerDiedDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sounds")
	USoundBase* ShootSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sounds")
	USoundBase* DieSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveTriggered(const FInputActionValue& Value);
	void MoveCompleted(const FInputActionValue& Value);

	void Shoot(const FInputActionValue& Value);

	bool IsInMapBoundsHorizontal(float XPos) const;
	bool IsInMapBoundsVertical(float ZPos) const;

	void OnShootCooldownTimeout();
	
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
