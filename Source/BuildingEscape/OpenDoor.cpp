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

	/// TODO Avoid polling every tick
	if (!PressurePlate) { return; }
	// Check if weight on pressure plate is sufficient to trigger.
	if (GetTotalWeightOnPlate() >= GetTriggerWeight()) { OnOpen.Broadcast(); }
	else { OnClose.Broadcast(); }
}

