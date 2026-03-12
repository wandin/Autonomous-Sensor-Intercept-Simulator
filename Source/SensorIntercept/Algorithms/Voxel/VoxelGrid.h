#pragma once

#include "CoreMinimal.h"

class FVoxelGrid
{
public:
	static void Filter(const TArray<FVector>& InputPoints, float VoxelSize, TArray<FVector>& OutputPoints);	
};
