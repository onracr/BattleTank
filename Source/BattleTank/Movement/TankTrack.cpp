// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"

UTankTrack::UTankTrack()
{
    PrimaryComponentTick.bCanEverTick = false;

}

void UTankTrack::BeginPlay()
{
    Super::BeginPlay();
    OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    DriveTrack();
    ApplySidewaysForce();
    CurrentThrottle = .0f;
}

void UTankTrack::SetThrottle(float Throttle)
{
    CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1.f, 1.f);
    // FVector ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
    // FVector ForceLocation = GetComponentLocation();

    // UPrimitiveComponent *TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    // TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

void UTankTrack::DriveTrack()
{
    FVector ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
    FVector ForceLocation = GetComponentLocation();

    UPrimitiveComponent *TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

void UTankTrack::ApplySidewaysForce()
{
    // Work-out the required acceleration this frame to correct
    float SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

    float DeltaTime = GetWorld()->GetDeltaSeconds();
    FVector CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();

    // Calculate and apply sideways (F=ma)
    UPrimitiveComponent *TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    FVector CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2; // two tracks

    TankRoot->AddForce(CorrectionForce);
}