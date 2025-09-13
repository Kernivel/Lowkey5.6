#pragma once

#include "AccessoryType.generated.h"

UENUM(BlueprintType)
enum class EAccessoryType : uint8
{
	Magazine,
	Grip,
	Stock,
	Handguard,
	RedDot,
	Scope,
	UnderBarrel,
	Muzzle,
	None UMETA(Hidden, DisplayName = "None")
};