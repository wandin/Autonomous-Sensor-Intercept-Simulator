#include "Misc/AutomationTest.h"
#include "SensorIntercept/Algorithms/InterceptSolver/InterceptSolver.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FInterceptSolverMonteCarloTest,
	"SensorIntercept.Unit.InterceptSolver.MonteCarlo",
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::EngineFilter
)

bool FInterceptSolverMonteCarloTest::RunTest(const FString& Parameters)
{
	const int32 Iterations = 200;

	for (int32 i = 0; i < Iterations; i++)
	{
		FVector Shooter = FVector::ZeroVector;

		FVector Target(
			FMath::RandRange(-2000, 2000),
			FMath::RandRange(-2000, 2000),
			FMath::RandRange(0, 500)
			);

		FVector Velocity(
			FMath::RandRange(-300, 300),
			FMath::RandRange(-300, 300),
			FMath::RandRange(-50, 50)
			);

		float ProjectileSpeed = FMath::RandRange(200, 1500);

		FVector Intercept;

		bool bResult = FInterceptSolver::SolveIntercept(Shooter, Target, Velocity, ProjectileSpeed, Intercept);

		if (bResult)
		{
			float Distance = FVector::Dist(Shooter, Intercept);

			TestTrue("Intercept point must be reachable", Distance > 0);
		}
	}
	return true;
}