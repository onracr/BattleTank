// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()

private:
	float CurrentThrottle = 0.f;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void DriveTrack();
	void ApplySidewaysForce();

public:
	UTankTrack();

	// Sets a throttle between -1 and +1
	UFUNCTION(BlueprintCallable, Category = Input) 
	void SetThrottle(float Throttle);

	// Max force per track, In Newtons
	UPROPERTY(EditDefaultsOnly)
	float TrackMaxDrivingForce = 400000;	// assume 40 tonne tank, and 1g acceleration

protected:
	virtual void BeginPlay() override;

};
