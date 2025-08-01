#include "Utils/MathUtils.h"

FTransform MathUtils::AddVectorToTransformLocation(const FTransform& Transform, const FVector& Vector)
{
	FTransform Result = Transform;
	Result.SetLocation(Result.GetLocation() + Vector);
	return Result;
}


FTransform MathUtils::AddVectorToTransformRotation(const FTransform& Transform, const FVector& Vector)
{
	FTransform Result = Transform;
	FRotator Rot(Vector.X, Vector.Y, Vector.Z);
	FQuat RotQuat = FQuat(Rot).GetNormalized();
	Result.SetRotation(Result.GetRotation() * RotQuat);
	return Result;
}
FTransform MathUtils::AddRotatorToTransform(const FTransform& Transform, const FRotator& Rotator)
{
	FTransform Result = Transform;
	Result.SetRotation((Result.GetRotation() * FQuat(Rotator)).GetNormalized());
	return Result;
}