#pragma once
#include "WeaponFireMode.generated.h"

UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	Single UMETA(DisplayName = "Single Shot"),
	Burst UMETA(DisplayName = "Burst Fire"),
	Automatic UMETA(DisplayName = "Automatic Fire"),
	Charge UMETA(DisplayName = "Charge Shot"),
	None UMETA(Hidden, DisplayName = "None")
};