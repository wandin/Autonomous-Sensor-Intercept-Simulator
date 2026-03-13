#include "KalmanFilter.h"

void FKalmanFilter::Initialize(const FVector& InitialPosition)
{
	Position = InitialPosition;
	Velocity = FVector::ZeroVector;
	bInitialized = true;
}

void FKalmanFilter::Predict(float DeltaTime)
{
	if (!bInitialized)
	{
		return;
	}
	Position += Velocity * DeltaTime;
}

void FKalmanFilter::Update(const FVector& Measurement)
{
	if (!bInitialized)
	{
		return;
	}
	FVector Innovation  = Measurement - Position;

	float Gain = 0.5f;

	Position += Gain * Innovation;
	Velocity += Gain * Innovation;
}

FVector FKalmanFilter::GetPosition() const
{
	return Position;
}

FVector FKalmanFilter::GetVelocity() const
{
	return Velocity;
}
