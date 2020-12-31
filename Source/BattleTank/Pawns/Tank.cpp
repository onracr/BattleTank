// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATank::BeginPlay()
{
	Super::BeginPlay();	// Needed for BP BeginPlay() to run!!
	CurrentHealth = StartingHealth;
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ATank::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);

	CurrentHealth -= DamageToApply; 
	if (CurrentHealth <= 0)
		OnDeath.Broadcast();

	return DamageToApply;
}

float ATank::GetHealthPercentage() const 
{
	return (float)CurrentHealth / (float)StartingHealth;
}