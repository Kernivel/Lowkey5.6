#pragma once
#include "CoreMinimal.h"

class MathUtils
{
public:

	/* Additions */
	static FTransform AddVectorToTransformLocation(const FTransform& Transform, const FVector& Vector);
	static FTransform AddVectorToTransformRotation(const FTransform& Transform, const FVector& Vector);
	static FTransform AddRotatorToTransform(const FTransform& Transform, const FRotator& Rotator);
};

