#include "ClusterUtils.h"

void FClusterUtils::DetectClusters(const TArray<FVector>& Points, float ClusterDistance, int32 MinClusterSize,
	int32 MaxClusterSize, float GroundHeightThreshold, TArray<FLidarCluster>& OutClusters)
{
   OutClusters.Empty();

    if (Points.Num() == 0)
    {
        return;
    }

    TArray<bool> Visited;
    Visited.Init(false, Points.Num());

    for (int32 i = 0; i < Points.Num(); i++)
    {
        if (Visited[i] || Points[i].Z < GroundHeightThreshold)
        {
            continue;
        }

        FLidarCluster Cluster;
        TArray<int32> Stack;
        Stack.Add(i);

        while (Stack.Num() > 0)
        {
            const int32 Index = Stack.Pop();

            if (Visited[Index])
            {
                continue;
            }

            Visited[Index] = true;

            const FVector& Point = Points[Index];
            if (Point.Z < GroundHeightThreshold)
            {
                continue;
            }

            Cluster.Points.Add(Point);

            for (int32 j = 0; j < Points.Num(); j++)
            {
                if (Visited[j])
                {
                    continue;
                }

                const FVector& OtherPoint = Points[j];
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

            OutClusters.Add(Cluster);
        }
    }
}
