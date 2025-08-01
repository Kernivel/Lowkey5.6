// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemObject.h"
#include "Weapon/WeaponType.h"
#include "Weapon/Ammo/AmmoStruct.h"
#include "AmmoObject.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class MYPROJECT_API UAmmoObject : public UItemObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo|Mesh")
	TSoftObjectPtr<UStaticMesh> SoftStaticMeshReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo|Stats")
	uint8 AmmoCount = 30; // The amount of ammo in this object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo|Stats")
	uint8 MaxAmmoCount = 180; // The maximum amount of ammo this object can hold
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo|Stats")
	EWeaponType WeaponType = EWeaponType::None; // The type of weapon this ammo is for

	UFUNCTION(BlueprintCallable, Category = "Ammo|Initialization")
	bool InitializeData(const FAmmoData& AmmoData);

};
