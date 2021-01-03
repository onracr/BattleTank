// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"
#include "../Actors/SprungWheel.h"
#include "../Components/SpawnPoint.h"


UTankTrack::UTankTrack()
{
    PrimaryComponentTick.bCanEverTick = false;

}

void UTankTrack::SetThrottle(float Throttle)
{
    float CurThrottle = FMath::Clamp<float>(Throttle, -1.f, 1.f);
   
    DriveTrack(CurThrottle);
}

void UTankTrack::DriveTrack(float CurThrottle)
{
    float ForceApplied = CurThrottle * TrackMaxDrivingForce;
    
    auto Wheels = GetWheels();
    auto ForcePerWheel = ForceApplied / Wheels.Num();

    for (auto Wheel : Wheels)
    {
        Wheel->AddDrivingForce(ForcePerWheel);
    }
}

TArray<ASprungWheel *> UTankTrack::GetWheels() const
{
   TArray<ASprungWheel*> ResultArray;
   TArray<USceneComponent*> Children;

   GetChildrenComponents(true, Children);

    for (auto Child : Children)
    {
        auto SpawnPointChild = Cast<USpawnPoint>(Child);
        if (!SpawnPointChild) continue;
    
        AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();
        
        auto SprungWheel = Cast<ASprungWheel>(SpawnedChild);
        if (!SprungWheel) continue;
    
        ResultArray.Add(SprungWheel);
    }

   return ResultArray;
}
