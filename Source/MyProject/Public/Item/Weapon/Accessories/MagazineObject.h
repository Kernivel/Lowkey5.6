#pragma once
#include "Item/Weapon/Accessories/AccessoryObject.h"
#include "MagazineObject.generated.h"

UCLASS(BlueprintType)
class UMagazineObject :
    public UAccessoryObject
{
	GENERATED_BODY()
public:
	uint8 MagazineSize = 30; // The size of the magazine
};

