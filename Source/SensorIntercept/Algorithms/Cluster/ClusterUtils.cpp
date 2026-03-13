#include "ClusterUtils.h"


namespace
{
	FIntVector GetCellKey(const FVector& Point, const float CellSize)
	{
		return FIntVector(
			FMath::FloorToInt(Point.X / CellSize),
			FMath::FloorToInt(Point.Y / CellSize),
			FMath::FloorToInt(Point.Z / CellSize)
			);
	}

	void BuildSpatialGrid(const TArray<FVector>& Points, const float CellSize, TMap<FIntVector, TArray<int32>>& OutGrid)
	{
		OutGrid.Empty();

		for (int32 Index = 0; Index < Points.Num(); ++Index)
		{
			const FIntVector CellKey = GetCellKey(Points[Index], CellSize);
			OutGrid.FindOrAdd(CellKey).Add(Index);
		}
	}

	void GatherNeighborIndices(const FVector& Point, const float CellSize, const TMap<FIntVector, TArray<int32>>& Grid, TArray<int32>& OutNeighborIndices)
	{
		OutNeighborIndices.Reset();

		const FIntVector BaseCell = GetCellKey(Point, CellSize);

		for (int32 DX = -1; DX <= 1; ++DX)
		{
			for (int32 DY = -1; DY <= 1; ++DY)
			{
				for (int32 DZ = -1; DZ <= 1; ++DZ)
				{
					const FIntVector NeighborCell(BaseCell.X + DX, BaseCell.Y + DY, BaseCell.Z + DZ);

					if (const TArray<int32>* CellIndices = Grid.Find(NeighborCell))
					{
						OutNeighborIndices.Append(*CellIndices);
					}
				}
			}
		}
	}

	void FinalizeCluster(FLidarCluster& Cluster)
	{
		check(Cluster.Points.Num() > 0)
		{
			FVector Sum = FVector::ZeroVector;
			FVector Min = Cluster.Points[0];
			FVector Max = Cluster.Points[0];

			for (const FVector& Point : Cluster.Points)
			{
				Sum += Point;

				Min.X = FMath::Min(Min.X, Point.X);
				Min.Y = FMath::Min(Min.Y, Point.Y);
				Min.Z = FMath::Min(Min.Z, Point.Z);

				Max.X = FMath::Max(Max.X, Point.X);
				Max.Y = FMath::Max(Max.Y, Point.Y);
				Max.Z = FMath::Max(Max.Z, Point.Z);
			}

			Cluster.Centroid = Sum / static_cast<float>(Cluster.Points.Num());
			Cluster.MinBounds = Min;
			Cluster.MaxBounds = Max;
		}
	}
}

void FClusterUtils::DetectClusters(const TArray<FVector>& Points, float ClusterDistance, int32 MinClusterSize,
	int32 MaxClusterSize, float GroundHeightThreshold, TArray<FLidarCluster>& OutClusters)
{
	OutClusters.Empty();

	if (Points.IsEmpty() || ClusterDistance <= 0.0f || MinClusterSize <= 0 || MaxClusterSize <= MinClusterSize)
	{
		return;
	}

	// Cell size equal to cluster distance is a good first approximation
	const float CellSize = ClusterDistance;

	TMap<FIntVector, TArray<int32>> SpatialGrid;
	BuildSpatialGrid(Points, CellSize, SpatialGrid);

	TArray<bool> Visited;
	Visited.Init(false, Points.Num());

	TArray<int32> Stack;
	TArray<int32> NeighborIndices;

	for (int32 StartIndex = 0; StartIndex < Points.Num(); ++StartIndex)
	{
		if (Visited[StartIndex] || Points[StartIndex].Z < GroundHeightThreshold)
		{
			continue;
		}

		FLidarCluster Cluster;
		Stack.Reset();
		Stack.Add(StartIndex);

		while (!Stack.IsEmpty())
		{
			const int32 CurrentIndex = Stack.Pop(EAllowShrinking::No);

			if (Visited[CurrentIndex])
			{
				continue;
			}
			Visited[CurrentIndex] = true;

			const FVector& CurrentPoint = Points[CurrentIndex];
			if (CurrentPoint.Z < GroundHeightThreshold)
			{
				continue;
			}

			Cluster.Points.Add(CurrentPoint);

			GatherNeighborIndices(CurrentPoint, CellSize, SpatialGrid, NeighborIndices);

			for (const int32 NeighborIndex : NeighborIndices)
			{
				if (Visited[NeighborIndex])
				{
					continue;
				}

				const FVector& NeighborPoint = Points[NeighborIndex];
				if (NeighborPoint.Z < GroundHeightThreshold)
				{
					continue;
				}

				if (FVector::DistSquared(CurrentPoint, NeighborPoint) <= FMath::Square(ClusterDistance))
				{
					Stack.Add(NeighborIndex);
				}
			}
		}
		if (Cluster.Points.Num() >= MinClusterSize && Cluster.Points.Num() <= MaxClusterSize)
		{
			FinalizeCluster(Cluster);
			OutClusters.Add(MoveTemp(Cluster));
		}
	}
}
