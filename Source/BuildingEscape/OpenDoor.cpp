// Copyright JJCraig 2018

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (playerController) {
		TriggerActor = playerController->GetPawn();
	}

}

void UOpenDoor::OpenDoor() {

	AActor* Owner = GetOwner();
	FRotator openRotation = FRotator(0.0f, openAngle, 0.0f);
	Owner->SetActorRotation(openRotation);
}

void UOpenDoor::CloseDoor() {

	AActor* Owner = GetOwner();
	FRotator openRotation = FRotator(0.0f, 0.0f, 0.0f);
	Owner->SetActorRotation(openRotation);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(TriggerActor)) { OpenDoor(); }
	if (PressurePlate && !PressurePlate->IsOverlappingActor(TriggerActor)) { CloseDoor(); }
}

