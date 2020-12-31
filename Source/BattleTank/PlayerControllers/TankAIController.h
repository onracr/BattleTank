// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class UTankAimingComponent;

UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

private:
	APawn* PlayerPawn;
	APawn* AITank;
	UTankAimingComponent* AimingComponent;

	UPROPERTY(EditAnywhere, Category = Setup, meta = (AllowPrivateAccess = "true"))
	float AcceptanceRadius = 3000.f;	// how close AI Tanks get 

	virtual void SetPawn(APawn* InPawn) override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnPossessedTankDeath();

protected:
	virtual void BeginPlay() override;
	
};
