#pragma once

#include "Engine/DataTable.h"
#include "UObject/SoftObjectPtr.h"
#include "AccessoryType.h"
#include "AccessoryStruct.generated.h"

USTRUCT(BlueprintType)
struct FAccessory : public FTableRowBase
{
	GENERATED_BODY()
	/* Base informations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FString Name = TEXT("UnknownAccessory");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TSoftObjectPtr<UStaticMesh> StaticMesh; // Soft reference to the weapon mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
	TSoftObjectPtr<UTexture2D> Image; // Soft reference to the weapon image
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	EAccessoryType Type = EAccessoryType::None;
	/* Damage variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Damage")
	int FlatDamage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Damage")
	float MultDamage = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|FireRate")
	int FlatFireRate = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|FireRate")
	float MultFireRate = 1;
	/* Magazine variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Magazine|Size")
	uint8 MagazineSize = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Magazine|ReloadSpeed")
	float MultReloadSpeed = 1;

	FAccessory()
	{
		Name = TEXT("Default Accessory");
		Type = EAccessoryType::None;
		StaticMesh = nullptr; // Default to nullptr if no mesh is set
		Image = nullptr; // Default to nullptr if no image is set
	}
};