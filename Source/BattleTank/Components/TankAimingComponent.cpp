// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "../Projectiles/Projectile.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Kismet/GameplayStatics.h"

#define OUT

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

}

void UTankAimingComponent::BeginPlay()
{
	// First fire is after initial reload
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (RoundsLeft == 0)
	{
		FiringState = EFiringState::OutOfAmmo;
		return;
	}
	if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
		FiringState = EFiringState::Reloading;
	else if (IsBarrelMoving())
		FiringState = EFiringState::Aiming;
	else
		FiringState = EFiringState::Locked;
}

void UTankAimingComponent::Initialize(UTankBarrel *BarrelToSet, UTankTurret *TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

int32 UTankAimingComponent::GetRoundsLeft() const 
{
	return RoundsLeft;
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel)) return;

	FVector LaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("ProjectileSocket"));

	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		GetWorld(),
		OUT LaunchVelocity,	// Velocity!
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0.f,
		0.f,
		ESuggestProjVelocityTraceOption::DoNotTrace
		);

	if (bHaveAimSolution)
	{
		AimDirection = LaunchVelocity.GetSafeNormal();	// Gets the direction of the vector you provide (the unit vector)
		MoveBarrelTowards(AimDirection);
	}else{
		UE_LOG(LogTemp, Warning, TEXT("%s does not have an aim solution!"), *GetOwner()->GetName())
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDir)
{
	if (!ensure(Barrel && Turret)) return;

	FRotator BarrelRotation = Barrel->GetForwardVector().Rotation();	// Look furthermore for BarrelRotation concept!
	FRotator AimRotator = AimDir.Rotation();
	FRotator DeltaRotator = AimRotator - BarrelRotation;

	Barrel->Elevate(DeltaRotator.Pitch);

	if (FMath::Abs(DeltaRotator.Yaw) > 180.f)
		DeltaRotator *= -1.f;

	Turret->Rotate(DeltaRotator.Yaw);
}

void UTankAimingComponent::Fire()
{
	if (ensure(Barrel))
	{
		if (FiringState != EFiringState::Reloading && FiringState != EFiringState::OutOfAmmo)
		{
			FVector2D ScreenLocation;

			FVector ProjectileLocation = Barrel->GetSocketLocation(FName("ProjectileSocket"));
			FRotator ProjectileRotation = Barrel->GetSocketRotation(FName("ProjectileSocket"));

			GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(ProjectileLocation, OUT ScreenLocation);
			AProjectile *SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, ProjectileLocation, ProjectileRotation);
			if (ensure(SpawnedProjectile))
			{
				SpawnedProjectile->LaunchProjectile(LaunchSpeed);
				LastFireTime = FPlatformTime::Seconds();
				RoundsLeft--;
			}
		}
	}
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) return false;
	return !(Barrel->GetForwardVector().Equals(AimDirection, .01f));
}