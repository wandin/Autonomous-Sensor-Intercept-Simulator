#include "Misc/AutomationTest.h"
#include "SensorIntercept/Algorithms/Cluster/ClusterUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FClusterUtilsStressTest,
	"SensorIntercept.Unit.ClusterUtils.Stress",
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::EngineFilter
)

bool FClusterUtilsStressTest::RunTest(const FString& Parameters)
{
	const int32 PointCount = 10000;

	TArray<FVector> Points;
	Points.Reserve(PointCount);

	// Generating synthetic LIDAR Cloud
	for (int32 i = 0; i < PointCount; i++)
	{
		Points.Add(FVector(
			FMath::RandRange(-5000, 5000),
			FMath::RandRange(-5000, 5000),
			FMath::RandRange(0, 300)
			));
	}

    TArray<FLidarCluster> Clusters;

	double StartTime = FPlatformTime::Seconds();

	FClusterUtils::DetectClusters(Points, 100.0f, 10, 100, 100.0f, Clusters);

	double Duration = FPlatformTime::Seconds() - StartTime;

	UE_LOG(LogTemp, Display, TEXT("Cluster stress test: %f seconds"), Duration);

	TestTrue("Clustering should complete under 0.1 seconds", Duration < 0.1);

	return true;
}