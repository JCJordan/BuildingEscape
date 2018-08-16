// Copyright JJCraig 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()
		
public:	
	// Sets default values for this component's properties
	UOpenDoor();
	float GetTotalWeightOnPlate();
	float GetTriggerWeight();

	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnOpen;

	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnClose;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	/** Trigger Volume that will activate door opening */
	UPROPERTY(editAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	/** Mass required to activate PressurePlate under normal gravity (in kg) */
	UPROPERTY(editInstanceOnly)
	float TriggerMass = 50.0f;

	AActor* Owner = nullptr;
				
};