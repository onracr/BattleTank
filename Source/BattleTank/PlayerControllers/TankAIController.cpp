// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "../Components/TankAimingComponent.h"
#include "../Pawns/Tank.h"  // So we can implement OnDeath !
#include "Engine/World.h"

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();

    PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    AITank = GetPawn() ? GetPawn() : nullptr;
    AimingComponent = AITank->FindComponentByClass<UTankAimingComponent>();
}

void ATankAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!ensureAlways(PlayerPawn && AITank)) return;
    if (!ensureAlways(AimingComponent)) return;

    MoveToActor(PlayerPawn, AcceptanceRadius);

    FVector HitLocation = PlayerPawn->GetActorLocation();

    AimingComponent->AimAt(HitLocation);

    if (AimingComponent->GetFiringState() == EFiringState::Locked)
    {
        AimingComponent->Fire();
    }
}

void ATankAIController::SetPawn(APawn* InPawn)  // Gets in-game Pawn via Super Class
{
    Super::SetPawn(InPawn);

    if (InPawn)
    {
        auto PossessedTank = Cast<ATank>(InPawn);
        if (!ensure(PossessedTank)) return;

        // TODO Subscribe our local method to the tank's death event
        PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
    }
}

void ATankAIController::OnPossessedTankDeath()
{
    UE_LOG(LogTemp, Warning, TEXT("AI Tank's dead!"))
    AITank->DetachFromControllerPendingDestroy();
}