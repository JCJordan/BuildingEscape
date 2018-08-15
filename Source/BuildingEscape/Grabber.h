// Copyright JJCraig 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	/** Reach distance from actor center point */
	UPROPERTY(editAnywhere)
	float reachDistance = 100.0f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	FHitResult InteractObject;

	FVector GrabLocation;

	void Grab();
	void Release();
	void InteractCheck();
	void FindPhysicsHandleComponent();
	void SetupInputComponent();
	void GetGrabTraceStartAndEnd(FVector& Start, FVector& End);
	
};
