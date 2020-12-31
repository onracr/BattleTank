// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class UTankAimingComponent;

/**
 * Responsible for helping the player aim.
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	UTankAimingComponent* AimingComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Aim Settings", meta = (AllowPrivateAccess = "true"))
	float CrosshairXLocation = .5f;
	UPROPERTY(EditDefaultsOnly, Category = "Aim Settings", meta = (AllowPrivateAccess = "true"))
	float CrosshairYLocation = .3333f;
	UPROPERTY(EditDefaultsOnly, Category = "Aim Settings", meta = (AllowPrivateAccess = "true"))
	float LineTraceRange = 1000000.f;

	void AimTowardsCrosshair();
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;
	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;
	bool GetLookVectorHitLocation(FVector& HitLocation, FVector& LookDirection) const;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION()
	void OnPlayerDeath();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category=Setup)
	void FoundAimingComponent(UTankAimingComponent* AimingComponentReference);
};
