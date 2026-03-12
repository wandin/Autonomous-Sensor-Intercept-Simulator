#pragma once

#include "CoreMinimal.h"
#include "ClusterTypes.h"

class FClusterUtils
{
public:

	static void DetectClusters(const TArray<FVector>& Points, float ClusterDistance, int32 MinClusterSize, int32 MaxClusterSize, float GroundHeightThreshold, TArray<FLidarCluster>& OutClusters);
	
};
