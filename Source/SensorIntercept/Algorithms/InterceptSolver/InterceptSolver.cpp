#include "InterceptSolver.h"

bool FInterceptSolver::SolveIntercept(const FVector& ShooterPos, const FVector& TargetPos, const FVector& TargetVelocity, float InProjectileSpeed, FVector& OutInterceptPoint)
{
	if (ShooterPos.Equals(TargetPos, KINDA_SMALL_NUMBER))
	{
		OutInterceptPoint = TargetPos;
		return true;
	}
	
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
	
	float t = TNumericLimits<float>::Max();

	if (t1 > 0)
	{
		t = FMath::Min(t, t1);
	}
	if (t2 > 0)
	{
		t = FMath::Min(t, t2);
	}
	if (t == TNumericLimits<float>::Max())
	{
		return false;
	}

	OutInterceptPoint = TargetPos + TargetVelocity * t;
	return true;
}
