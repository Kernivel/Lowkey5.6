#pragma once
#include "CoreMinimal.h"
#include "Curves/CurveVector.h"
#include "CurvePlayBack.generated.h"
USTRUCT(BlueprintType)
struct FCurvePlayBack
{
	GENERATED_BODY()
	
	// ADD UPROPERTY() to ensure garbage collection works correctly
	UPROPERTY()
	UCurveVector* CCameraRotationCurve = nullptr;
	UPROPERTY()
	UCurveVector* CCameraLocationCurve = nullptr;
	UPROPERTY()
	UCurveVector* CHandRotationCurve = nullptr;
	UPROPERTY()
	UCurveVector* CHandLocationCurve = nullptr;
	float CurrentTime = 0.0f;
	float PlayRate = 1.0f;
	bool bIsPlaying = false;

	/* Default constructor, should be avoided */
	FCurvePlayBack()
		: CCameraRotationCurve(nullptr), CCameraLocationCurve(nullptr),
		CHandRotationCurve(nullptr), CHandLocationCurve(nullptr),
		CurrentTime(0.0f), PlayRate(1.0f), bIsPlaying(true)
	{
	}

	
	/* Do not destroy UCurves in the destructor as they are handled by Unreal garbage collector once marked as UPROPERTY */
	~FCurvePlayBack() = default;


	FCurvePlayBack(
		UCurveVector* InCameraCurveRotation,
		UCurveVector* InCameraCurveLocation,
		UCurveVector* InHandCurveRotation,
		UCurveVector* InHandCurveLocation)
		: CCameraRotationCurve(InCameraCurveRotation)
		, CCameraLocationCurve(InCameraCurveLocation)
		, CHandRotationCurve(InHandCurveRotation)
		, CHandLocationCurve(InHandCurveLocation)
		, CurrentTime(0.0f)
		, PlayRate(3.5f)
		, bIsPlaying(true)
	{
	}

	bool GetCurveIsOver() const
	{

		if (CCameraRotationCurve != nullptr)
		{
			float MinTime = 0.0f; // Assuming you want to check from the start of the curve
			float MaxTime = 0.0f; // This will be set by the curve itself
			CCameraRotationCurve->GetTimeRange(MinTime, MaxTime);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Curve min: ") + FString::FromFloat(this->CurrentTime));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Curve min: ") + FString::FromFloat(MinTime) + TEXT(" Curve max : ") + FString::FromFloat(MaxTime));

			if(this->CurrentTime >= MaxTime)
			{
				return true;
			}
		}
		return false;
	}
};