#pragma once

#include "CoreMinimal.h"

#include "ClusterTypes.generated.h"

USTRUCT()
struct FLidarCluster
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FVector> Points;

	UPROPERTY()
	FVector Centroid = FVector::ZeroVector;

	UPROPERTY()
	FVector MinBounds = FVector::ZeroVector;

	UPROPERTY()
	FVector MaxBounds = FVector::ZeroVector;
};