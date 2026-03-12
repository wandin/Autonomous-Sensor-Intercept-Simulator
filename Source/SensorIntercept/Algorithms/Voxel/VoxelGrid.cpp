#include "VoxelGrid.h"

void FVoxelGrid::Filter(const TArray<FVector>& InputPoints, float VoxelSize, TArray<FVector>& OutputPoints)
{
	TMap<FIntVector, FVector> VoxelMap;

	for (const FVector& Point : InputPoints)
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
	VoxelMap.GenerateValueArray(OutputPoints);
}
