// Copyright JJCraig 2018

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"
#include "Containers/Array.h"
#include "Components/PrimitiveComponent.h"


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
	if (!Owner) { UE_LOG(LogTemp, Error, TEXT("Owner not found in Open Door component!")); return; }
	if (!PressurePlate) { UE_LOG(LogTemp, Error, TEXT("PressurePlate not found on %s!"), *(GetOwner()->GetName())); return; }
	currentDoorState = defaultDoorState;

}

void UOpenDoor::OpenDoor() {

	if (!Owner) { return; }
	FRotator OpenRotation = FRotator(0.0f, OpenAngle, 0.0f);
	Owner->SetActorRotation(OpenRotation);
	currentDoorState = DoorState::OPEN;
	LastDoorOpenTime = GetWorld()->GetTimeSeconds();

	return; 

}

void UOpenDoor::CloseDoor() {

	if (!Owner) { return; }
	FRotator CloseRotation = FRotator(0.0f, 0.0f, 0.0f);
	Owner->SetActorRotation(CloseRotation);
	currentDoorState = DoorState::CLOSED;

	return; 
}

// Gets total weight on pressure plate under 'current' gravity
float UOpenDoor::GetTotalWeightOnPlate() {

	TArray<AActor*> OverlappingActors;
	float TotalMassOnPlate = 0.0f;

	if (!PressurePlate) { return TotalMassOnPlate; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (const AActor* Actor : OverlappingActors) {
		TotalMassOnPlate += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMassOnPlate * (GetWorld()->GetGravityZ() * -1);		
}

// Gets Trigger Weight required using 'default' gravity
float UOpenDoor::GetTriggerWeight() {

	return TriggerMass * (GetWorld()->GetDefaultGravityZ() * -1);	

}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PressurePlate) { return; }
	// Check if the door is currently closed, there is a registered pressure plate, and is the player stood on the pressure plate.
	if (PressurePlate && (GetTotalWeightOnPlate() >= GetTriggerWeight())) { OpenDoor(); }
	// Check if the door is currently open, there is a registered pressure plate, the player is not stood on the pressure plate, and the delay time has run out.
	if ((currentDoorState == DoorState::OPEN) && PressurePlate && (GetTotalWeightOnPlate() < GetTriggerWeight()) && (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > CloseDoorDelay)) { CloseDoor(); }
}

