// Copyright (c) 2026 Wanderson Lopes. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LidarSensorComponent.generated.h"


USTRUCT()
struct FLidarCluster
{
	GENERATED_BODY()

	TArray<FVector> Points;

	FVector Centroid = FVector::ZeroVector;
	FVector MinBounds = FVector::ZeroVector;
	FVector MaxBounds = FVector::ZeroVector;
};

USTRUCT()
struct FTrackedObject
{
	GENERATED_BODY()

	int32 Id = -1;

	FVector Position = FVector::ZeroVector;
	FVector Velocity = FVector::ZeroVector;

	float LastSeenTime = 0.0f;

	FVector BoundsMin;
	FVector BoundsMax;
};

UCLASS(ClassGroup=(Sensors), meta=(BlueprintSpawnableComponent))
class SENSORINTERCEPT_API ULidarSensorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULidarSensorComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void PerformScan(float BaseAngle);
	
	void AccumulateScan();

	void DrawPointCloud();

	void ApplyVoxelFilter();

	void DetectClusters();
	void DrawClusters();

	void TrackClusters();
	void DrawTrackedObjects();
	
	void ComputeIntercepts();
	FColor GetDepthColor(float Distance) const;
	
	// Lidar
	UPROPERTY(EditAnywhere, Category="Lidar")
	float ScanRadius = 1500.0f;
	
	UPROPERTY(EditAnywhere, Category="Lidar")
	int32 RayCount = 120;

	UPROPERTY(EditAnywhere, Category="Lidar")
	bool bDrawDebug = true;

	UPROPERTY(EditAnywhere, Category="Lidar")
	int32 VerticalLayers = 50;

	UPROPERTY(EditAnywhere, Category="Lidar")
	float VerticalFOV = 70.0f;

	UPROPERTY(EditAnywhere, Category="Lidar")
	int32 MaxMapPoints = 10000;

	UPROPERTY(EditAnywhere, Category="Lidar", meta=(Units="cm"))
	float VoxelSize = 10.0f;
	
	// Sweep properties
	UPROPERTY(EditAnywhere, Category="Sweep")
	float SweepSpeed = 45.0f;

	UPROPERTY(EditAnywhere, Category="Sweep")
	int32 RaysPerStep = 6;

	float CurrentScanAngle = 0.0f;
	
	UPROPERTY()
	TArray<FVector> CurrentScan;

	UPROPERTY()
	TArray<FVector> MapPointCloud;
	
	// Cluster
	UPROPERTY(EditAnywhere, Category="Clustering")
	float ClusterDistance = 45.0f;

	UPROPERTY(EditAnywhere, Category="Clustering")
	int32 MinClusterSize = 10;

	UPROPERTY(EditAnywhere, Category="Clustering")
	int32 MaxClusterSize = 2000;

	UPROPERTY(EditAnywhere, Category="Clustering", meta=(Units="cm"))
	float GroundHeightThreshold = 5.0f;
	
	UPROPERTY()
	TArray<FLidarCluster> Clusters;


	// Tracked objects
	UPROPERTY()
	TArray<FTrackedObject> TrackedObjects;

	int32 NextObjectId = 0;

	UPROPERTY(EditAnywhere, Category="Tracking", meta=(Units="cm"))
	float TrackingDistanceThreshold = 100.0f;


	// Projectile
	UPROPERTY(EditAnywhere, Category="Intercept")
	float ProjectileSpeed = 1200.0f;
};
