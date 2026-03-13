#include "Misc/AutomationTest.h"
#include "SensorIntercept/Algorithms/Cluster/ClusterTypes.h"
#include "SensorIntercept/Algorithms/Cluster/ClusterUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FClusterUtilsGroundFilterTest,
	"SensorIntercept.Unit.ClusterUtils.GroundFilter",
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::EngineFilter
)

bool FClusterUtilsGroundFilterTest::RunTest(const FString& Parameters)
{
	TArray<FVector> InputPoints;

	//Ground Points
	InputPoints.Add(FVector(0, 0, -10));
	InputPoints.Add(FVector(10, 0, -5));
	InputPoints.Add(FVector(20, 0, -1));

	// Valid Cluster above ground
	InputPoints.Add(FVector(100, 100, 50));
	InputPoints.Add(FVector(110, 100, 50));
	InputPoints.Add(FVector(100, 110, 50));

	TArray<FLidarCluster> OutClusters;

	FClusterUtils::DetectClusters(InputPoints, 30.0f, 2, 100, 0.0f, OutClusters);

	TestEqual(TEXT("Should ignore ground points and detect one cluster"), OutClusters.Num(), 1);

	return true;
}