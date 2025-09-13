#pragma once
#include "Engine/DataTable.h"
#include "UObject/SoftObjectPtr.h"
#include "WeaponType.h"
#include "WeaponFireMode.h"
#include "WeaponStruct.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FString Name = TEXT("Unknown name");;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TSoftObjectPtr<USkeletalMesh> SkeletalMesh; // Soft reference to the weapon mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
	TSoftObjectPtr<UTexture2D> Image; // Soft reference to the weapon image
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	EWeaponType Type = EWeaponType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireMode")
	TArray<EWeaponFireMode> FireModes; // How the weapon can be fired
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Description")
	FText Description = FText::FromString(TEXT("No description"));
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Damage")
	int32 Damage = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Range")
	float Range = 100.0f; // Range in meters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Firerate")
	float FireRate = 300; //Bullets per minute
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Reload")
	float ReloadSpeedMultiplyer = 1; // Base animation will be multiplied by this value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Reload")
	uint8 AmmoCapacity = 10;

	FWeaponData()
	{
		FWeaponData::FireModes = { EWeaponFireMode::Single };
	}
};

