// Copyright (c) 2026 Wanderson Lopes. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SensorIntercept/Sensors/LidarSensorComponent.h"
#include "DroneActor.generated.h"

UCLASS()
class SENSORINTERCEPT_API ADroneActor : public AActor
{
	GENERATED_BODY()

public:
	ADroneActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere)
	ULidarSensorComponent* LidarSensor;
};
