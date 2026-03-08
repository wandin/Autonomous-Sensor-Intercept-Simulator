// Copyright (c) 2026 Wanderson Lopes. All rights reserved.


#include "MovingTarget.h"

#include "VectorTypes.h"
#include "Math/UnitConversion.h"


AMovingTarget::AMovingTarget()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	MeshComponent->SetCollisionProfileName("BlockAll");
}

void AMovingTarget::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	LinearDirection = MovementDirection.GetSafeNormal();

	if (LinearDirection.IsNearlyZero())
	{
		LinearDirection = FVector::ForwardVector;
	}

	CircleAxis = CircleAxis.GetSafeNormal();
	if (CircleAxis.IsNearlyZero())
	{
		CircleAxis = FVector::UpVector;
	}
}

void AMovingTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;

	switch (MotionType)
	{
	case ETargetMotionType::LinearPingPong:
		UpdateLinearPingPong(DeltaTime);
		break;

	case ETargetMotionType::Circular:
		UpdateCircular(DeltaTime);
		break;

	case ETargetMotionType::SineWave:
		UpdateSineWave(DeltaTime);
		break;

	case ETargetMotionType::Evasive:
		UpdateEvasive(DeltaTime);
		break;

	default:
		break;
	}
}

void AMovingTarget::UpdateLinearPingPong(float DeltaTime)
{
	FVector NewLocation = GetActorLocation() + LinearDirection * Speed * DeltaTime;

	const float SignedDistance = FVector::DotProduct(NewLocation - StartLocation,LinearDirection);

	if (FMath::Abs(SignedDistance) > MovementRange)
	{
		LinearDirection *= -1.0f;

		const float ClampedDistance = FMath::Clamp(SignedDistance, -MovementRange, MovementRange);
		NewLocation = StartLocation + LinearDirection * ClampedDistance;
	}

	SetActorLocation(NewLocation);
}

void AMovingTarget::UpdateCircular(float DeltaTime)
{
	const float AngularSpeed = (CircleRadius > KINDA_SMALL_NUMBER) ? (Speed / CircleRadius) : 0.0f;
	const float Angle = ElapsedTime * AngularSpeed;

	FVector BasisX = FVector::ForwardVector;
	if (FMath::Abs(FVector::DotProduct(BasisX, CircleAxis)) > 0.99f)
	{
		BasisX = FVector::RightVector;
	}

	BasisX = (BasisX - FVector::DotProduct(BasisX, CircleAxis) * CircleAxis).GetSafeNormal();
	const FVector BasisY = FVector::CrossProduct(CircleAxis, BasisX).GetSafeNormal();

	const FVector Offset = BasisX * FMath::Cos(Angle) * CircleRadius + BasisY * FMath::Sin(Angle) * CircleRadius;

	SetActorLocation(StartLocation + Offset);
}

void AMovingTarget::UpdateSineWave(float DeltaTime)
{
	const FVector ForwardOffset = LinearDirection * Speed * ElapsedTime;

	FVector SideAxis = FVector::CrossProduct(FVector::UpVector, LinearDirection).GetSafeNormal();
	if (SideAxis.IsNearlyZero())
	{
		SideAxis = FVector::RightVector;
	}

	const float SineOffset = FMath::Sin(ElapsedTime * SineFrequency * 2.0f * PI) * SineAmplitude;

	const FVector NewLocation = StartLocation + ForwardOffset + SideAxis * SineOffset;
	SetActorLocation(NewLocation);
}

void AMovingTarget::UpdateEvasive(float DeltaTime)
{
	const FVector ForwardOffset = LinearDirection * Speed * ElapsedTime;

	FVector SideAxis = FVector::CrossProduct(FVector::UpVector, LinearDirection).GetSafeNormal();
	if (SideAxis.IsNearlyZero())
	{
		SideAxis = FVector::RightVector;
	}

	const float FastOsc = FMath::Sin(ElapsedTime * EvasiveFrequency * 2.0f * PI) * EvasiveAmplitude;
	const float SlowOsc = FMath::Sin(ElapsedTime * 0.5f * EvasiveFrequency * 2.0f * PI) * (EvasiveAmplitude * 0.4f);

	const FVector NewLocation =
		StartLocation +
		ForwardOffset +
		SideAxis * FastOsc +
		FVector::UpVector * SlowOsc;

	SetActorLocation(NewLocation);
	
}