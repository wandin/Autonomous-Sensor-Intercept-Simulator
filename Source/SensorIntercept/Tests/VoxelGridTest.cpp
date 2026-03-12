#include "Misc/AutomationTest.h"
#include "SensorIntercept/Algorithms/Voxel/VoxelGrid.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FVoxelGridTest,
	"SensorIntercept.Unit.VoxelGrid.BasicFilter",
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::EngineFilter
)

bool FVoxelGridTest::RunTest(const FString& Parameters)
{
	TArray<FVector> Input;

	Input.Add(FVector(0, 0, 0));
	Input.Add(FVector(1,1,1));
	Input.Add(FVector(2,2,2));

	Input.Add(FVector(100,100,100));
	Input.Add(FVector(101,101,101));

	TArray<FVector> Output;

	FVoxelGrid::Filter(Input, 50.f, Output);

	TestTrue("Voxel filter reduces points", Output.Num() < Input.Num());

	return true;
}