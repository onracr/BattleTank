// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

// Enum for aiming states
UENUM()
enum class EFiringState : uint8
{
	Reloading,
	Locked,
	Aiming,
	OutOfAmmo
};

class UTankBarrel;
class UTankTurret;

UCLASS(Blueprintable)
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UTankBarrel* Barrel;
	UTankTurret* Turret;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<class AProjectile> ProjectileBlueprint;

	UPROPERTY(EditAnywhere, Category = Firing, meta = (AllowPrivateAccess = "true"))
	float LaunchSpeed = 100000.f; // 1000 m/s

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = Firing, meta = (AllowPrivateAccess = "true"))
	float ReloadTimeInSeconds = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = Firing, meta = (AllowPrivateAccess = "true"))
	uint32 RoundsLeft = 3;

	double LastFireTime = .0f;

	FVector AimDirection;

	void MoveBarrelTowards(FVector AimDir);
	bool IsBarrelMoving();

public:	
	UTankAimingComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initialize(UTankBarrel *BarrelToSet, class UTankTurret *TurretToSet);

	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintCallable, Category=Firing)
	int32 GetRoundsLeft() const;

	void AimAt(FVector HitLocation);
	EFiringState GetFiringState() const;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = State)
	EFiringState FiringState = EFiringState::Aiming;
	
};
