// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemObject.h"
#include "Item/Weapon/WeaponType.h"
#include "Item/Weapon/Ammo/AmmoStruct.h"
#include "Net/UnrealNetwork.h"
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
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Ammo|Stats")
	uint8 AmmoCount = 30; // The amount of ammo in this object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo|Stats")
	uint8 MaxAmmoCount = 180; // The maximum amount of ammo this object can hold
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "Ammo|Stats")
	EWeaponType WeaponType = EWeaponType::None; // The type of weapon this ammo is for

	UFUNCTION(BlueprintCallable, Category = "Ammo|Initialization")
	bool InitializeData(const FAmmoData& AmmoData);

protected:
	/* Override to enable replication on specified props */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* Called when ammo is replicated on a client/server ? */
	UFUNCTION()
	void OnRep_AmmoCount();

	UFUNCTION()
	void OnRep_WeaponType();
};
