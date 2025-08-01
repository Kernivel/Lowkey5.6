#pragma once
#include "Camera/CameraShakeBase.h"
#include "Shakes/PerlinNoiseCameraShakePattern.h"
#include "LightWeaponFireCameraShake.generated.h"

UCLASS()
class ULightWeaponFireCameraShake : public UCameraShakeBase
{
	GENERATED_BODY()
public:
	ULightWeaponFireCameraShake(const FObjectInitializer& ObjectInitializer);
};

