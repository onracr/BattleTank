// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTurret.h"

void UTankTurret::Rotate(float RelativeSpeed)
{
    RelativeSpeed = FMath::Clamp(RelativeSpeed, -1.f, 1.f);

    float RotationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
    float NewRotation = GetRelativeRotation().Yaw + RotationChange;

    SetRelativeRotation(FRotator(0.f, NewRotation, 0.f));
}