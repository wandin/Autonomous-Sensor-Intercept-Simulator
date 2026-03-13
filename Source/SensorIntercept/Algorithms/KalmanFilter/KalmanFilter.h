#pragma once

#include "CoreMinimal.h"

class FKalmanFilter
{
public:
	void Initialize(const FVector& InitialPosition);

	void Predict(float DeltaTime);

	void Update(const FVector& Measurement);

	FVector GetPosition() const;
	FVector GetVelocity() const;

private:

	FVector Position = FVector::ZeroVector;
	FVector Velocity = FVector::ZeroVector;

	float ProcessNoise = 1.0f;
	float MeasurementNoise = 10.0f;

	bool bInitialized = false;
};
