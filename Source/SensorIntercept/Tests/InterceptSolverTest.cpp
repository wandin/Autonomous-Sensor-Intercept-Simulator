#include "Misc/AutomationTest.h"
#include "SensorIntercept/Algorithms/InterceptSolver.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FInterceptSolverTest,
	"SensorIntercept.Unit.InterceptSolver.BasicIntercept",
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::EngineFilter
)

bool FInterceptSolverTest::RunTest(const FString& Parameters)
{
	FVector Shooter = FVector(0, 0, 0);
	FVector Target = FVector(1000, 0, 0);
	FVector Velocity = FVector(100, 0, 0);

	float ProjectileSpeed = 500.0f;

	FVector Intercept;

	bool bResult = FInterceptSolver::SolveIntercept(Shooter, Target, Velocity, ProjectileSpeed, Intercept);

	TestTrue("Intercept solution exists", bResult);

	TestTrue("Intercept point ahead of target", Intercept.X > Target.X);

	return true;
}