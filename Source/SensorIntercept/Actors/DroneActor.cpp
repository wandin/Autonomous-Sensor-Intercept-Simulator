// Copyright (c) 2026 Wanderson Lopes. All rights reserved.


#include "DroneActor.h"


ADroneActor::ADroneActor()
{
	PrimaryActorTick.bCanEverTick = true;

	LidarSensor = CreateDefaultSubobject<ULidarSensorComponent>(TEXT("LidarSensor"));
}

void ADroneActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADroneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator Rot = GetActorRotation();
	Rot.Yaw += 30.0f * DeltaTime;
	SetActorRotation(Rot);
}