// Copyright JJCraig 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UENUM()
enum class DoorState { OPEN, CLOSED };

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()
		
public:	
	// Sets default values for this component's properties
	UOpenDoor();
	void OpenDoor();
	void CloseDoor();
	void SetTriggerActor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	/** How many degrees connected door will open when activated */
	UPROPERTY(editAnywhere)
	float OpenAngle = 80.0f;

	/** Trigger Volume that will activate door opening */
	UPROPERTY(editAnywhere)
	ATriggerVolume* PressurePlate;

	/** Actor that PressurePlate (Trigger volume) will detect */
	UPROPERTY(editAnywhere)
	AActor* TriggerActor;

	/** Time after TriggerActor leaves PressurePlate connected door will stay open for (in seconds) */
	UPROPERTY(editAnywhere)
	float CloseDoorDelay = 1.f;



	float LastDoorOpenTime;
	AActor* Owner;

	/** Starting door state */
	UPROPERTY(editInstanceOnly)
	DoorState defaultDoorState = DoorState::CLOSED;
	DoorState currentDoorState;
				
};