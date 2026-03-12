#pragma once

#include "CoreMinimal.h"

class FInterceptSolver
{
public:

	static bool SolveIntercept(const FVector& ShooterPos, const FVector& TargetPos, const FVector& TargetVelocity, float InProjectileSpeed, FVector& OutInterceptPoint);
};
