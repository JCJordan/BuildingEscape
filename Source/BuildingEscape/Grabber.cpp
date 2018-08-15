// Copyright JJCraig 2018

#include "Grabber.h"
#include "Gameframework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("Grabber for [%s] is active and ready"), *(GetOwner()->GetName()))
	FindPhysicsHandleComponent();
	SetupInputComponent();	

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//UE_LOG(LogTemp, Warning, TEXT("Player View is from %s and is %s"), *(ViewLocation.ToString()), *(ViewRotation.ToString()))
	InteractCheck();
	
	if (PhysicsHandle->GrabbedComponent) {
		// Move Grabbed item
		PhysicsHandle->SetTargetLocation(GrabLocation);
	}

}

// Check for any interactable objects and mark if found.
void UGrabber::InteractCheck() {

	FVector GrabTraceStart, GrabTraceEnd;
	GetGrabTraceStartAndEnd(OUT GrabTraceStart, OUT GrabTraceEnd);

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT InteractObject, GrabTraceStart, GrabTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	if (InteractObject.GetActor()) {
		// TODO Mark found object
		UE_LOG(LogTemp, Warning, TEXT("Interactable object found within reach: %s"), *(InteractObject.GetActor()->GetName()))
	}
	
	return;

}

void UGrabber::GetGrabTraceStartAndEnd(OUT FVector& GrabTraceStart, OUT FVector& GrabTraceEnd) {

	FVector ViewLocation;
	FRotator ViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT ViewLocation, OUT ViewRotation);

	GrabTraceStart = ViewLocation;
	GrabTraceEnd = ViewLocation + (ViewRotation.Vector() * reachDistance);
	return;
}

// Pick up object within range
void UGrabber::Grab() {

	if (InteractObject.GetActor()) {
		UPrimitiveComponent* ComponentToGrab = InteractObject.GetComponent();

		PhysicsHandle->GrabComponent(
			ComponentToGrab, // Component Reference
			NAME_None, // Bone Name
			ComponentToGrab->GetOwner()->GetActorLocation(), // Component Location
			true // Rotation enabled?
		);
	}

	return;
}

// Release held component
void UGrabber::Release() {

	PhysicsHandle->ReleaseComponent();
	return;
}

// Check if there is a Physics Handle component and complain if not
void UGrabber::FindPhysicsHandleComponent() {

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) { UE_LOG(LogTemp, Error, TEXT("Physics handle component not found on %s"), *(GetOwner()->GetName())) }

	return;

}

// Check if there is a Input component and complain if not
void UGrabber::SetupInputComponent() {

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else { UE_LOG(LogTemp, Error, TEXT("Input component not found on %s"), *(GetOwner()->GetName())) }
	
	return;

}