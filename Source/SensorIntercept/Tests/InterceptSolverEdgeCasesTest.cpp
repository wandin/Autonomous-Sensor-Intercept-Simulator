#include "Misc/AutomationTest.h"
#include "SensorIntercept/Algorithms/InterceptSolver/InterceptSolver.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FInterceptSolverEdgeCasesTest,
	"SensorIntercept.Unit.InterceptSolver.EdgeCases",
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::EngineFilter
)

bool FInterceptSolverEdgeCasesTest::RunTest(const FString& Parameters)
{
	FVector Intercept;
	{
	//Case 1 - Target Stopped
	FVector Shooter(0,0,0);
	FVector Target(1000,0,0);
	FVector Velocity(0,0,0);

	bool bResult = FInterceptSolver::SolveIntercept(Shooter, Target, Velocity, 500.0f, Intercept);

	TestTrue("Stopped target should be interceptable", bResult);
	TestTrue("Intercept should equal target position", Intercept.Equals(Target, 1.0f));
	}
	// Case 2 - Projectile too slow

	{
		FVector Shooter(0,0,0);
		FVector Target(1000,0,0);
		FVector Velocity(600,0,0);

		bool bResult = FInterceptSolver::SolveIntercept(Shooter, Target, Velocity, 200.0f, Intercept);
		TestFalse("Projectile slower than target should fail", bResult);
	}

	// Case 3 - Perpendicular motion
	{
		FVector Shooter(0,0,0);
		FVector Target(1000,0,0);
		FVector Velocity(0,200,0);

		bool bResult = FInterceptSolver::SolveIntercept(Shooter, Target, Velocity, 800.0f, Intercept);

		TestTrue("Perpendicular motion should be interceptable", bResult);
		TestTrue("Perpendicular should be ahead of target", Intercept.Y > 0);
	}

	// Case 4 - Shooter already at target
	{
		FVector Shooter(0,0,0);
		FVector Target(0,0,0);
		FVector Velocity(100,0,0);

		bool bResult = FInterceptSolver::SolveIntercept(Shooter, Target, Velocity, 800.0f, Intercept);

		TestTrue("Immediate intercept should succeed", bResult);
	}
	return true;
}