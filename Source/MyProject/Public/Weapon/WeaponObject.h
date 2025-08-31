// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemObject.h"
#include "WeaponStruct.h"
#include "WeaponAnimationStruct.h"
#include "WeaponType.h"
#include "Accessories/AccessoryObject.h"
#include "Accessories/MagazineObject.h"
#include "Animation/AnimInstance.h"
#include "WeaponObject.generated.h"


UCLASS(BlueprintType)
class MYPROJECT_API UWeaponObject : public UItemObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessory|General")
	FString WeaponId; // Unique identifier for the weapon
	//Soft reference to the skeletal mesh of the weapon that can be loaded at runtime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Mesh")
	TSoftObjectPtr<USkeletalMesh> SoftSkelMeshReference;
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool InitializeData(const FWeaponData& WeaponData, FString RowId);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool InitializeAnimationData(const FWeaponAnimation& AnimationData);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FORCEINLINE EWeaponType GetWeaponType() const { return Type; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation")
	TSoftClassPtr <UAnimInstance> AnimBPClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float DelayBetweenShots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	EWeaponType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float BaseDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float BaseReloadSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float BaseFireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float BaseRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	uint8 AmmoCapacity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	uint8 CurrentAmmo; // Current ammo in the weapon, can be used to track ammo during gameplay
	// Must be pointers for Ubjects.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Accessories")
	TMap<FString, UAccessoryObject*> MainAccessories;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Accessories")
	TMap<FString, UAccessoryObject*> OptionnalAccessories;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Magazine")
	UMagazineObject* Magazine;

protected:

	float CalculateDelayBetweenShots(float FireRate) const
	{
		if(FireRate <= 0.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("FireRate is zero or negative, returning default delay of 1 second."));
			return 1.0f; // Default delay if fire rate is invalid
		}
		return 60.0f / FireRate; // Convert fire rate to delay in seconds
	}

private:
};
