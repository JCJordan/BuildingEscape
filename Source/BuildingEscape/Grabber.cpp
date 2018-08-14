// Copyright JJCraig 2018

#include "Grabber.h"
#include "Gameframework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
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
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector ViewLocation;
	FRotator ViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT ViewLocation, OUT ViewRotation);
	//UE_LOG(LogTemp, Warning, TEXT("Player View is from %s and is %s"), *(ViewLocation.ToString()), *(ViewRotation.ToString()))

	DrawDebugLine(GetWorld(), ViewLocation, ViewLocation + (ViewRotation.Vector() * reachDistance), FColor(0,0,0), false, 5.0f);

}
