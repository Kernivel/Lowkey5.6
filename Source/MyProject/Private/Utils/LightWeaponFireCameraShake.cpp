#include "Utils/LightWeaponFireCameraShake.h"


ULightWeaponFireCameraShake::ULightWeaponFireCameraShake(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Setup du pattern (exemple pour PerlinNoise)
    UPerlinNoiseCameraShakePattern* Pattern = ObjectInitializer.CreateDefaultSubobject<UPerlinNoiseCameraShakePattern>(this, TEXT("PerlinPattern"));
    // Réglez vos paramètres ici
	Pattern->RotationAmplitudeMultiplier = 0.3f;
    Pattern->RotationFrequencyMultiplier = 14.f;
	//Pattern->FOV->AmplitudeMultiplier = 0.2f;
	//Pattern->FOV->FrequencyMultiplier = 14.f;
    Pattern->Duration = 0.12f;
	Pattern->BlendInTime = 0.01f;
    Pattern->BlendOutTime = 0.05f;
    SetRootShakePattern(Pattern);
}