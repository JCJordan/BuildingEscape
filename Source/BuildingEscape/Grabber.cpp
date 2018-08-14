// Copyright JJCraig 2018

#include "Grabber.h"
#include "Gameframework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
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

	UE_LOG(LogTemp, Warning, TEXT("Grabber for [%s] is active and ready"), *(GetOwner()->GetName()))

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) { UE_LOG(LogTemp, Error, TEXT("Physics handle component not found on %s"), *(GetOwner()->GetName())) }

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) { 
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else { UE_LOG(LogTemp, Error, TEXT("Input component not found on %s"), *(GetOwner()->GetName())) }

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//UE_LOG(LogTemp, Warning, TEXT("Player View is from %s and is %s"), *(ViewLocation.ToString()), *(ViewRotation.ToString()))
	AActor* GrabItem;
	GrabItem = GrabCheck();	

}

AActor* UGrabber::GrabCheck() {

	FVector ViewLocation;
	FRotator ViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT ViewLocation, OUT ViewRotation);
	FVector ViewTraceEnd = ViewLocation + (ViewRotation.Vector() * reachDistance);

	DrawDebugLine(GetWorld(), ViewLocation, ViewTraceEnd, FColor(0, 0, 0), false, 0.0f, 0.f, 5.0f);

	FHitResult HitObject;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT HitObject, ViewLocation, ViewTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);
	AActor* HitActor = HitObject.GetActor();
	if (HitActor) {
		UE_LOG(LogTemp, Warning, TEXT("Actor hit by line trace: %s"), *HitActor->GetName())
		return HitActor;
	}
	return nullptr;

}

void UGrabber::Grab() {

	return;
}

void UGrabber::Release() {

	return;
}

