// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrel.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{
    RelativeSpeed = FMath::Clamp(RelativeSpeed, -1.f, 1.f);
    float ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
    float RawNewElevation = FMath::Clamp(
        GetRelativeRotation().Pitch + ElevationChange, 
        MinElevationDegrees, 
        MaxElevationDegrees
        );

    SetRelativeRotation(FRotator(RawNewElevation, 0.f, 0.f));
}
