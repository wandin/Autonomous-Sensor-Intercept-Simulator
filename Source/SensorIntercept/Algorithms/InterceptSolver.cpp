#include "InterceptSolver.h"

bool FInterceptSolver::SolveIntercept(const FVector& ShooterPos, const FVector& TargetPos, const FVector& TargetVelocity, float InProjectileSpeed, FVector& OutInterceptPoint)
{
	FVector R = TargetPos - ShooterPos;

	float a = FVector::DotProduct(TargetVelocity, TargetVelocity) - InProjectileSpeed * InProjectileSpeed;
	float b  = 2.0f * FVector::DotProduct(R, TargetVelocity);
	float c  = FVector::DotProduct(R, R);

	float Discriminant = b * b - 4 * a * c;

	if (Discriminant < 0)
	{
		return false;
	}

	float sqrtDiscriminant = FMath::Sqrt(Discriminant);

	float t1 = (-b + sqrtDiscriminant) / (2 * a);
	float t2 = (-b - sqrtDiscriminant) / (2 * a);

	float t = FMath::Min(t1, t2);

	if (t < 0)
	{
		t = FMath::Max(t1, t2);
	}
	if (t < 0)
	{
		return false;
	}

	OutInterceptPoint = TargetPos + TargetVelocity * t;
	return true;
}
