// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "../Components/TankAimingComponent.h"
#include "../Pawns/Tank.h"
#include "Engine/World.h"

void ATankPlayerController::BeginPlay() 
{
    Super::BeginPlay();

    if (!GetPawn()) return;

    AimingComp = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if (ensure(AimingComp))
        FoundAimingComponent(AimingComp);
}

void ATankPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AimTowardsCrosshair();
}

void ATankPlayerController::SetPawn(APawn* InPawn)
{
    Super::SetPawn(InPawn);

    if (InPawn)
    {
        auto PlayerTank = Cast<ATank>(InPawn);
        if (!ensure(PlayerTank)) return;

        PlayerTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPlayerDeath);
    }
}

void ATankPlayerController::OnPlayerDeath()
{
    UE_LOG(LogTemp, Warning, TEXT("Player's Dead!"))
    StartSpectatingOnly();
}

void ATankPlayerController::AimTowardsCrosshair()
{   
    // 4 line in beginplay was here.

    FVector HitLocation;
    if (GetSightRayHitLocation(HitLocation))
    {
        if (ensure(AimingComp))
            AimingComp->AimAt(HitLocation);
    }
}

bool ATankPlayerController::GetSightRayHitLocation(FVector &OutHitLocation) const
{
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);

    FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);

    FVector WorldDirection;

    if (GetLookDirection(ScreenLocation, WorldDirection))
    {
        return GetLookVectorHitLocation(OutHitLocation, WorldDirection);
    }
    return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
    FVector WorldLocation;  // to be discarded
    return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector& HitLocation, FVector& LookDirection) const
{
    FHitResult HitResult;
    FVector StartLocation = PlayerCameraManager->GetCameraLocation();
    FVector LineTraceEnd = StartLocation + (LookDirection * LineTraceRange);

    FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

    if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, LineTraceEnd, ECollisionChannel::ECC_Camera, TraceParams))
    {
        HitLocation = HitResult.Location;
        return true;
    }
    HitLocation = FVector(0.f);
    return false;
}