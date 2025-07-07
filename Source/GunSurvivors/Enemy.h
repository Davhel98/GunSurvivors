// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "Engine/TimerHandle.h"

#include "TopDownCharacter.h"

#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDiedDelegate);
UCLASS()
class GUNSURVIVORS_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* EnemyFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* DeadFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ATopDownCharacter* PlayerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanFollow = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float MovementSpeed = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float StopDistance = 20.0f;

	FTimerHandle DestroyTimer;

	FEnemyDiedDelegate EnemyDiedDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound")
	USoundBase* DieSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Die();

	void OnDestroyTimerTimeout();
};
