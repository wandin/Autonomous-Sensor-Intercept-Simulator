#include "Misc/AutomationTest.h"
#include "SensorIntercept/Algorithms/Cluster/ClusterTypes.h"
#include "SensorIntercept/Algorithms/Cluster/ClusterUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FClusterUtilsTest,
	"SensorIntercept.Unit.ClusterUtils.BasicDetection",
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::EngineFilter
)

bool FClusterUtilsTest::RunTest(const FString& Parameters)
{
	TArray<FVector> InputPoints;

	// Cluster 1
	InputPoints.Add(FVector(100, 100, 50));
	InputPoints.Add(FVector(110, 100, 50));
	InputPoints.Add(FVector(100, 110, 50));

	// Cluster 2
	InputPoints.Add(FVector(500, 500, 50));
	InputPoints.Add(FVector(510, 500, 50));
	InputPoints.Add(FVector(500, 510, 50));

	TArray<FLidarCluster> OutClusters;

	FClusterUtils::DetectClusters(
		InputPoints,
		30.0f,   // ClusterDistance
		2,       // MinClusterSize
		100,     // MaxClusterSize
		0.0f,    // GroundHeightThreshold
		OutClusters
	);

	TestEqual(TEXT("Should detect two clusters"), OutClusters.Num(), 2);

	return true;
}