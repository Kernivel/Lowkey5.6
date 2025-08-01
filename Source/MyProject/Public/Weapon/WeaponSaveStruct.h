#pragma once

#include "WeaponSaveStruct.generated.h"


USTRUCT(BlueprintType)
struct FAccessorySaveData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category="Saved|Accessory")
	FString AccessoryId;

	FAccessorySaveData() : AccessoryId(TEXT("NULL")) {}
};

USTRUCT(BlueprintType)
struct FWeaponSaveData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "Saved|Weapon")
	FString WeaponId;
	UPROPERTY(EditAnywhere, Category = "Saved|Weapon")
	TArray<FAccessorySaveData> MainAccessories;
	UPROPERTY(EditAnywhere, Category = "Saved|Weapon")
	TArray<FAccessorySaveData> OptionnalAccessories;
	UPROPERTY(EditAnywhere, Category = "Saved|Weapon")
	FAccessorySaveData Magazine; // Magazine is a special accessory
	
	FWeaponSaveData() : WeaponId(TEXT("NULL")), Magazine(FAccessorySaveData()) {}
};
