#pragma once

#include "WeaponType.generated.h"
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AssaultRifle UMETA(DisplayName = "AR"),
	Pistol UMETA(DisplayName = "Pistol"),
	SubmachineGun UMETA(DisplayName = "SMG"),
	None UMETA(Hidden, DisplayName = "None")
};