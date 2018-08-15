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

	Owner = GetOwner();
	currentDoorState = defaultDoorState;
	SetTriggerActor();

}

// Set Actor that will activate pressure plate
void UOpenDoor::SetTriggerActor() {
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (playerController) {
		TriggerActor = playerController->GetPawn();
	}

	return;
}

void UOpenDoor::OpenDoor() {

	FRotator OpenRotation = FRotator(0.0f, OpenAngle, 0.0f);
	Owner->SetActorRotation(OpenRotation);
	currentDoorState = DoorState::OPEN;
	LastDoorOpenTime = GetWorld()->GetTimeSeconds();
}

void UOpenDoor::CloseDoor() {

	FRotator CloseRotation = FRotator(0.0f, 0.0f, 0.0f);
	Owner->SetActorRotation(CloseRotation);
	currentDoorState = DoorState::CLOSED;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check if the door is currently closed, there is a registered pressure plate, and is the player stood on the pressure plate.
	if (PressurePlate && PressurePlate->IsOverlappingActor(TriggerActor)) { OpenDoor(); }
	// Check if the door is currently open, there is a registered pressure plate, the player is not stood on the pressure plate, and the delay time has run out.
	if ((currentDoorState == DoorState::OPEN) && PressurePlate && !PressurePlate->IsOverlappingActor(TriggerActor) && (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > CloseDoorDelay)) { CloseDoor(); }
}

