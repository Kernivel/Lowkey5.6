// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemObject.h"
#include "Weapon/Accessories/AccessoryType.h"
#include "AccessoryObject.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MYPROJECT_API UAccessoryObject : public UItemObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessory|General")
	FString AccessoryId; // Unique identifier for the accessory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessory|Mesh")
	TSoftObjectPtr<UStaticMesh> SoftStaticMeshReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessory|Type")
	EAccessoryType Type = EAccessoryType::None; // The type of accessory
};
