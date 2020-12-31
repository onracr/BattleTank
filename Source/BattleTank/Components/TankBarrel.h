// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"


// UCLASS macro makes a acomponent blueprint spawnable with 'meta = (BlueprintSpawnableComponent)'
 
UCLASS()
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = Setup, meta = (AllowPrivateAccess = "true"))
	float MaxDegreesPerSecond = 5.f;
	UPROPERTY(EditDefaultsOnly, Category = Setup, meta = (AllowPrivateAccess = "true"))
	float MaxElevationDegrees = 30.f;
	UPROPERTY(EditDefaultsOnly, Category = Setup, meta = (AllowPrivateAccess = "true"))
	float MinElevationDegrees = 0.f;

public:
	void Elevate(float RelativeSpeed);

};
