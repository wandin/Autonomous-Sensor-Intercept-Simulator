// Copyright (c) 2026 Wanderson Lopes. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingTarget.generated.h"

UENUM(BlueprintType)
enum class ETargetMotionType : uint8
{
	LinearPingPong		UMETA(DisplayName="Linear Ping Pong"),
	Circular			UMETA(DisplayName="Circular"),
	SineWave			UMETA(DisplayName="Sine Wave"),
	Evasive				UMETA(DisplayName="Evasive")
};

UCLASS()
class SENSORINTERCEPT_API AMovingTarget : public AActor
{
	GENERATED_BODY()

public:
	AMovingTarget();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	void UpdateLinearPingPong(float DeltaTime);
	void UpdateCircular(float DeltaTime);
	void UpdateSineWave(float DeltaTime);
	void UpdateEvasive(float DeltaTime);

private:
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category="Movement")
	ETargetMotionType MotionType = ETargetMotionType::LinearPingPong;

	UPROPERTY(EditAnywhere, Category="Movement")
	float Speed = 200.0f;

	UPROPERTY(EditAnywhere, Category="Movement")
	FVector MovementDirection = FVector(1.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category="Movement")
	float MovementRange = 800.0f;

	UPROPERTY(EditAnywhere, Category="Movement|Circular")
	float CircleRadius = 400.0f;

	UPROPERTY(EditAnywhere, Category="Movement|Circular")
	FVector CircleAxis = FVector::UpVector;

	UPROPERTY(EditAnywhere, Category="Movement|Sine")
	float SineAmplitude = 200.0f;

	UPROPERTY(EditAnywhere, Category="Movement|Sine")
	float SineFrequency = 1.0f;

	UPROPERTY(EditAnywhere, Category="Movement|Evasive")
	float EvasiveAmplitude = 250.0f;

	UPROPERTY(EditAnywhere, Category="Movement|Evasive")
	float EvasiveFrequency = 2.0f;

	FVector StartLocation = FVector::ZeroVector;
	FVector LinearDirection = FVector::ZeroVector;

	float ElapsedTime = 0.0f;
};
