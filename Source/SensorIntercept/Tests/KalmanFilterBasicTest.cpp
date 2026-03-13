#include "Misc/AutomationTest.h"
#include "SensorIntercept/Algorithms/KalmanFilter/KalmanFilter.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FKalmanFilterBasicTest,
	"SensorIntercept.Unit.KalmanFilter.Basic",
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::EngineFilter
)

bool FKalmanFilterBasicTest::RunTest(const FString& Parameters)
{
	FKalmanFilter Filter;

	FVector Initial(0,0,0);
	Filter.Initialize(Initial);

	Filter.Predict(1.0f);

	FVector Measurement(10,0,0);

	Filter.Update(Measurement);

	FVector Pos = Filter.GetPosition();

	TestTrue(TEXT("Kalman position should move toward measurement"), Pos.X > 0);

	return true;
}