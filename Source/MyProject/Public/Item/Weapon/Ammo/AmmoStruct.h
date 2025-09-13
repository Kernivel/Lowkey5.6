#pragma once
#include "Engine/DataTable.h"
#include "UObject/SoftObjectPtr.h"
#include "Item/Weapon/WeaponType.h"
#include "AmmoStruct.generated.h"

USTRUCT(BlueprintType)
struct FAmmoData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FString Name = TEXT("Unknown Ammo");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TSoftObjectPtr<UStaticMesh> StaticMesh; // Soft reference to the ammo mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
	TSoftObjectPtr<UTexture2D> Image; // Soft reference to the ammo image
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	EWeaponType WeaponType = EWeaponType::None; // The type of weapon this ammo is for
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Description")
	FText Description = FText::FromString(TEXT("No description"));
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|AmmoCount")
	uint8 AmmoCount = 0; // The amount of ammo in this object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|MaxAmmoCount")
	uint8 MaxAmmoCount = 180; // The maximum amount of ammo this object can hold

	FAmmoData()
	{
		// Default constructor can initialize default values if needed
	}
};