// Copyright (c) 2026 Wanderson Lopes. All rights reserved.


#include "LidarSensorComponent.h"

#include "ImaginaryBlueprintData.h"
#include "SensorIntercept/Algorithms/InterceptSolver.h"

ULidarSensorComponent::ULidarSensorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULidarSensorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULidarSensorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentScan.Reset();

	CurrentScanAngle += SweepSpeed * DeltaTime;
	if (CurrentScanAngle > 360.0f)
	{
		CurrentScanAngle -= 360.0f;
	}
	
	PerformScan(CurrentScanAngle);
	DrawPointCloud();

	DetectClusters();
	TrackClusters();
	
	ComputeIntercepts();
	
	DrawClusters();
	DrawTrackedObjects();
}

void ULidarSensorComponent::PerformScan(float BaseAngle)
{
	AActor* Owner = GetOwner();
	if(!Owner)
	{
		return;
	}

	FVector Origin = Owner->GetActorLocation();
	
	if (VerticalLayers <= 1)
	{
		return;
	}
	
	for (int32 v = 0; v < VerticalLayers; v++)
	{
		
		float VerticalAngle = -VerticalFOV * 0.5f + VerticalFOV / (VerticalLayers - 1) * v;
		
		for (int32 i = 0; i <  RaysPerStep; i++)
		{
			float HorizontalAngle = BaseAngle + i * (360.0f / RayCount);

			FRotator RayRotation(VerticalAngle, HorizontalAngle, 0.0f);

			FVector Direction = RayRotation.Vector();
			FVector End = Origin + Direction * ScanRadius;

			FHitResult Hit;

			FCollisionQueryParams Params;
			Params.AddIgnoredActor(Owner);

			bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Origin, End, ECC_WorldStatic, Params);

			if (bHit)
			{
				CurrentScan.Add(Hit.Location);
				
				if (bDrawDebug)
				{
					float Distance = FVector::Dist(Origin, Hit.Location);
					FColor DepthColor = GetDepthColor(Distance);
					DrawDebugLine(GetWorld(), Origin, Hit.Location, DepthColor, false, 0.05f, 0, 1.0f);
				}
			}
			else if (bDrawDebug)
			{
				DrawDebugLine(GetWorld(), Origin, End, FColor::Green, false, 0.02f, 0, 0.2f);
			}
		}
	}
	AccumulateScan();
}

void ULidarSensorComponent::AccumulateScan()
{
	for (const FVector& Point : CurrentScan)
	{
		MapPointCloud.Add(Point);
	}

	if (MapPointCloud.Num() > MaxMapPoints)
	{
		int32 Overflow = MapPointCloud.Num() - MaxMapPoints;
		MapPointCloud.RemoveAt(0, Overflow, EAllowShrinking::No);
	}

	static int32 FrameCounter = 0;
	FrameCounter++;
	if (FrameCounter % 5 == 0)
	{
		ApplyVoxelFilter();
	}
}

void ULidarSensorComponent::DrawPointCloud()
{
	if (!bDrawDebug)
	{
		return;
	}

	AActor* Owner = GetOwner();
	if(!Owner)
	{
		return;
	}

	FVector Origin = Owner->GetActorLocation();

	for(const FVector& Point : MapPointCloud)
	{
		float Distance = FVector::Dist(Origin, Point);

		FColor DepthColor = GetDepthColor(Distance);

		DrawDebugPoint(GetWorld(), Point,4.0f, DepthColor,false,-1.0f);
	}
}

void ULidarSensorComponent::ApplyVoxelFilter()
{
	TMap<FIntVector, FVector> VoxelMap;

	for (const FVector& Point : MapPointCloud)
	{
		int32 X = FMath::FloorToInt(Point.X / VoxelSize);
		int32 Y = FMath::FloorToInt(Point.Y / VoxelSize);
		int32 Z = FMath::FloorToInt(Point.Z / VoxelSize);

		FIntVector Key(X, Y, Z);

		if (!VoxelMap.Contains(Key))
		{
			VoxelMap.Add(Key, Point);
		}
	}

	MapPointCloud.Empty();
	VoxelMap.GenerateValueArray(MapPointCloud);
}

void ULidarSensorComponent::DetectClusters()
{
	Clusters.Empty();

	TArray<bool> Visited;
	Visited.Init(false, MapPointCloud.Num());

	if (MapPointCloud.Num() == 0)
	{
		return;
	}
	
	for (int32 i = 0; i < MapPointCloud.Num(); i++)
	{
		if (Visited[i] || MapPointCloud[i].Z < GroundHeightThreshold)
		{
			continue;
		}

		FLidarCluster Cluster;
		TArray<int32> Stack;
		Stack.Add(i);

		while (Stack.Num() > 0)
		{
			int32 Index = Stack.Pop();

			if (Visited[Index])
			{
				continue;
			}

			Visited[Index] = true;

			const FVector& Point = MapPointCloud[Index];
			if (Point.Z < GroundHeightThreshold)
			{
				continue;
			}
			Cluster.Points.Add(Point);

			for (int32 j = 0; j < MapPointCloud.Num(); j++)
			{
				if (Visited[j])
				{
					continue;
				}

				const FVector& OtherPoint = MapPointCloud[j];
				if (OtherPoint.Z < GroundHeightThreshold)
				{
					continue;
				}
				if (FVector::Dist(Point, OtherPoint) < ClusterDistance)
				{
					Stack.Add(j);
				}
			}
		}

		if (Cluster.Points.Num() >= MinClusterSize && Cluster.Points.Num() <= MaxClusterSize)
		{
			FVector Sum = FVector::ZeroVector;

			FVector Min = Cluster.Points[0];
			FVector Max = Cluster.Points[0];

			for (const FVector& P : Cluster.Points)
			{
				Sum += P;

				Min.X = FMath::Min(Min.X, P.X);
				Min.Y = FMath::Min(Min.Y, P.Y);
				Min.Z = FMath::Min(Min.Z, P.Z);

				Max.X = FMath::Max(Max.X, P.X);
				Max.Y = FMath::Max(Max.Y, P.Y);
				Max.Z = FMath::Max(Max.Z, P.Z);
			}

			Cluster.Centroid = Sum / Cluster.Points.Num();
			Cluster.MinBounds = Min;
			Cluster.MaxBounds = Max;

			Clusters.Add(Cluster);
		}
	}
}

void ULidarSensorComponent::DrawClusters()
{
	if (!bDrawDebug)
	{
		return;
	}

	for (const FLidarCluster& Cluster : Clusters)
	{
		FVector Center = (Cluster.MinBounds + Cluster.MaxBounds) * 0.5f;
		FVector Extent = (Cluster.MaxBounds - Cluster.MinBounds) * 0.5f;

		
		DrawDebugBox(GetWorld(), Center, Extent, FColor::Red, false, 0.0f, 0, 2.0f);

		DrawDebugSphere(GetWorld(), Cluster.Centroid, 15.0f, 12, FColor::Yellow, false, 0.0f);
	}
}

void ULidarSensorComponent::TrackClusters()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	for (const FLidarCluster& Cluster : Clusters)
	{
		FVector Position = Cluster.Centroid;

		FTrackedObject* BestMatch = nullptr;
		float BestDistance = TrackingDistanceThreshold;

		for (FTrackedObject& Object : TrackedObjects)
		{
			float Distance = FVector::Dist(Object.Position, Position);

			if (Distance < BestDistance)
			{
				BestDistance = Distance;
				BestMatch = &Object;
			}
		}

		if (BestMatch)
		{
			float DeltaTime = CurrentTime - BestMatch->LastSeenTime;

			if (DeltaTime > KINDA_SMALL_NUMBER)
			{
				FVector NewVelocity = (Position - BestMatch->Position) / DeltaTime;
				BestMatch->Velocity = NewVelocity;
			}

			BestMatch->Position = Position;
			BestMatch->BoundsMin = Cluster.MinBounds;
			BestMatch->BoundsMax = Cluster.MaxBounds;
			BestMatch->LastSeenTime = CurrentTime;
		}
		else
		{
			FTrackedObject NewObject;
			NewObject.Id = NextObjectId++;
			NewObject.Position = Position;
			NewObject.BoundsMin = Cluster.MinBounds;
			NewObject.BoundsMax = Cluster.MaxBounds;
			NewObject.LastSeenTime = CurrentTime;

			TrackedObjects.Add(NewObject);
		}
	}

	for (int32 i = TrackedObjects.Num() - 1; i >= 0; i--)
	{
		if (CurrentTime - TrackedObjects[i].LastSeenTime > 1.0f)
		{
			TrackedObjects.RemoveAt(i);
		}
	}
}

void ULidarSensorComponent::DrawTrackedObjects()
{
	if (!bDrawDebug)
	{
		return;
	}
	for (const FTrackedObject& Object : TrackedObjects)
	{
		FVector Start = Object.Position;
		FVector End = Start + Object.Velocity * 0.5f;

		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.0f, 0, 3.0f);
	}
}

void ULidarSensorComponent::ComputeIntercepts()
{
	FVector Shooter = GetOwner()->GetActorLocation();

	for (const FTrackedObject& Obj : TrackedObjects)
	{
		FVector InterceptPoint;

		bool bValid = FInterceptSolver::SolveIntercept(
			Shooter,
			Obj.Position,
			Obj.Velocity,
			ProjectileSpeed,
			InterceptPoint
		);

		if (bValid)
		{
			DrawDebugSphere(
				GetWorld(),
				InterceptPoint,
				20.0f,
				16,
				FColor::Purple,
				false,
				0.0f
			);

			DrawDebugLine(
				GetWorld(),
				Shooter,
				InterceptPoint,
				FColor::Purple,
				false,
				0.0f,
				0,
				2.0f
			);
		}
	}
}

FColor ULidarSensorComponent::GetDepthColor(float Distance) const
{
	float Normalized = FMath::Clamp(Distance / ScanRadius, 0.0f, 1.0f);
	
	float R = 1.0f - Normalized;	// red near
	float G = Normalized;			// green mid
	float B = Normalized * 0.5f;	// cyan far

	return FColor(R * 255.0f, G * 255.0f, B * 255.0f);
}
