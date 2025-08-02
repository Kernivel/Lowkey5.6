// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "Kismet/KismetMathLibrary.h"

/* Custom includes */
#include "Weapon/WeaponObject.h"
#include "ItemObject.h"
#include "Weapon/Ammo/AmmoObject.h"
#include "Item.h"
#include "Weapon/Weapon.h"
#include "Weapon/Ammo/Ammo.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	/*************************
	* Constructor            *
	**************************/
public:	
	// Sets default values for this component's properties
	UInventoryComponent();


	/*************************
	* Public Variables       *
	**************************/
public:
	/* Data Arrays */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (UItemObject))
	TArray<UItemObject*> InventoryItems; // Array to hold items in the inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (UWeaponObject))
	TArray<UWeaponObject*> InventoryWeapons; // Array to hold weapons in the inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (UAmmoObject))
	TArray<UAmmoObject*> InventoryAmmo; // Array to hold ammo in the inventory

	/* Spawned Reference Arrays */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<AWeapon*> SpawnedWeapons; // Array to hold spawned weapon actors

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int MaxWeaponsCount = 3; // Maximum number of weapons in the weapon inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int MaxItemsCount = 3; // Maximum number of weapons in the weapon inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int MaxAmmosCount = 8; // Maximum number of Ammos in the ammo inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* WeaponDataTable; // Data table containing weapon data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* WeaponAnimationTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	uint8 CurrentlyEquipedIndex = 0; // Currently equipped weapon object

	/*************************
	* Public Functions       *
	**************************/
public:
	/****************** Default functions **********************/
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/****************** Management functions *************/
	UFUNCTION(BlueprintCallable, Category = "Creation")
	UWeaponObject* CreateWeaponObject(UObject* Outer, uint8 RowId);
	UFUNCTION(BlueprintCallable, Category = "Creation")
	uint8 SpawnWeaponsFromInventory(AActor* Outer, bool FirstPersonView); // Spawn all weapons from the inventory
	UFUNCTION(BlueprintCallable, Category = "Creation")
	AWeapon* SpawnWeaponItem(AActor* Outer, UWeaponObject* WeaponData, bool FirstPersonView);
	UFUNCTION(BlueprintCallable, Category = "Selection")
	UWeaponObject* SwitchWeaponIndex(int32 Index); // Switch weapon by index in the inventory
	UFUNCTION(BlueprintCallable, Category = "Getter")
	uint8 GetAmmoOfType(const EWeaponType WeaponType) const; // Get the amount of ammo of a specific type
	UFUNCTION(BlueprintCallable, Category = "Fetch")
	uint8 RetreiveAmmoOfType(const EWeaponType WeaponType,const uint8 AmmoCount); // Find the ammo of a specific type and remove the specified amount from the inventory
	UFUNCTION(BlueprintCallable, Category = "Add")
	bool AddItemToInventory(AItem* Item); // Add an item to the inventory
	UFUNCTION(BlueprintCallable, Category = "Add")
	bool AddWeaponToInventoryWeapon(AWeapon* Weapon); // Add an item to the inventory
	UFUNCTION(BlueprintCallable, Category = "Add")
	bool AddAmmoToInventory(AAmmo* Ammo); // Add an item to the inventory
	/****************** Management end *************/
	/*************************
	* Protected Functions    *
	**************************/
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void CleanSpawnedWeapons(); // Clear the array of spawned weapons
public:	

};
