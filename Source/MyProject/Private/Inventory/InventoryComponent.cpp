// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/* Create a weapon object from a RowId in WeaponDataTable */
UWeaponObject* UInventoryComponent::CreateWeaponObject(UObject* outer, uint8 RowId)
{
	if(WeaponDataTable == nullptr) 
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponDataTable is not set!"));
		return nullptr;
	} else 
	{
		FWeaponData* WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(*FString::FromInt(RowId)), TEXT("Lookup Weapon Data"));
		if (WeaponData)
		{
			UWeaponObject* NewWeapon = NewObject<UWeaponObject>(outer);
			if (NewWeapon)
			{
				NewWeapon->InitializeData(*WeaponData);
				FWeaponAnimation* WeaponAnimation = WeaponAnimationTable->FindRow<FWeaponAnimation>(FName(*FString::FromInt(RowId)), TEXT("Lookup Weapon Animation"));
				if (WeaponAnimation != nullptr)
				{
					UE_LOG(LogTemp, Log, TEXT("Animation data found for RowId: %d"), RowId);
					NewWeapon->InitializeAnimationData(*WeaponAnimation);
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("No animation data found for RowId: %d"), RowId);
				}
				// Add the new weapon to the inventory
				this->InventoryWeapons.Add(NewWeapon);
				return NewWeapon;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No weapon data found for RowId: %d"), RowId);
		}
	}
	return nullptr;
}



/* Spawns all weapons from the Inventory*/
/* Outer should be the Actor to which the weapons will be attached */
uint8 UInventoryComponent::SpawnWeaponsFromInventory(AActor* Outer, bool FirstPersonView)
{	
	if (Outer == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Outer provided!"));
		return 0;
	}
	uint8 SpawnedWeaponsCount = 0;
	this->CleanSpawnedWeapons(); // Destroy and clear the array of spawned weapons
	// Foreach loop
	for(UWeaponObject* Weapon : this->InventoryWeapons)
	{
		AWeapon* SpawnedWeapon = this->SpawnWeaponItem(Outer, Weapon, FirstPersonView);
		this->SpawnedWeapons.Add(SpawnedWeapon); // Add the spawned weapon to the array
		SpawnedWeaponsCount++;
	}
	UE_LOG(LogTemp, Log, TEXT("Spawned %d weapons items"), SpawnedWeaponsCount);
	return SpawnedWeaponsCount;
}

/* Spawn a Weapon Actor in the world and attach it to the OuterActor */
AWeapon* UInventoryComponent::SpawnWeaponItem(AActor* Outer, UWeaponObject* WeaponData, bool FirstPersonView)
{
	if (Outer == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Outer provided!"));
		return 0;
	}
	if (WeaponData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponToSpawn is null! Cannot spawn weapon."));
		return nullptr;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Outer;
	SpawnParams.Instigator = Outer->GetInstigator();
	AWeapon* WeaponItem = Outer->GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass(), Outer->GetActorLocation(), Outer->GetActorRotation(), SpawnParams);
	if (WeaponItem)
	{
		/* This Initialize covers mesh spawn an animations attach */
		WeaponItem->Initialize(WeaponData, FirstPersonView);
		WeaponItem->AttachToActor(Outer, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		WeaponItem->bCanBePickedUp = false;
		WeaponItem->bIsPickedUp = true; // Mark the weapon as picked up
		WeaponItem->CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Disable collision for the weapon item
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn weapon item."));
	}
	return WeaponItem;
}

UWeaponObject* UInventoryComponent::SwitchWeaponIndex(int32 Index)
{
	if (Index < 0 || Index >= this->InventoryWeapons.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid weapon index: %d"), Index);
		this->CurrentlyEquipedIndex = Index;
		return nullptr;
	}
	this->CurrentlyEquipedIndex = Index;
	return this->InventoryWeapons[Index];
}

void UInventoryComponent::CleanSpawnedWeapons()
{
	for (AWeapon* Weapon : this->SpawnedWeapons)
	{
		if (Weapon)
		{
			Weapon->Destroy();
		}
	}
	this->SpawnedWeapons.Empty(); // Clear the array of spawned weapons
}

uint8 UInventoryComponent::GetAmmoOfType(const EWeaponType WeaponType) const
{
	uint8 TotalAmmo = 0;
	for (const UAmmoObject* Ammo : this->InventoryAmmo)
	{
		if (Ammo && Ammo->WeaponType == WeaponType)
		{
			TotalAmmo += Ammo->AmmoCount;
		}
	}
	return TotalAmmo;
}

uint8 UInventoryComponent::RetreiveAmmoOfType(const EWeaponType WeaponType,const uint8 AmmoCount)
{
	uint8 TotalAmmoRemoved = 0;
	for (int32 i = 0; i < this->InventoryAmmo.Num(); ++i)
	{
		UAmmoObject* Ammo = this->InventoryAmmo[i];
		if (Ammo && Ammo->WeaponType == WeaponType)
		{
			uint8 AmmoToRemove = AmmoCount - TotalAmmoRemoved;
			uint8 RemovableAmmo = FMath::Min(AmmoToRemove, Ammo->AmmoCount); // Ensure we don't remove more than available
			Ammo->AmmoCount -= RemovableAmmo; // Reduce the ammo count
			TotalAmmoRemoved += RemovableAmmo; // Increment the total ammo removed
			if (Ammo->AmmoCount <= 0)
			{
				this->InventoryAmmo.RemoveAt(i);
				--i; // Adjust index after removal
			}
			if (TotalAmmoRemoved >= AmmoCount)
			{
				break; // Stop if we have removed enough ammo
			}
		}
	}
	return TotalAmmoRemoved;
}
bool UInventoryComponent::AddWeaponToInventoryWeapon(AWeapon* Weapon)
{
	if (Weapon == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AddWeaponToInventoryWeapon: Cannot add null Weapon to inventory."));
		return false;
	}
	if (!IsValid(Weapon->ItemObject)) {
		UE_LOG(LogTemp, Error, TEXT("AddWeaponToInventoryWeapon: Invalid ItemObject. Cannot initialize animations."));
		return false; // Invalid item object
	}
	UWeaponObject* WeaponObject = Cast<UWeaponObject>(Weapon->ItemObject);
	if (!IsValid(WeaponObject))
	{
		UE_LOG(LogTemp, Error, TEXT("AddWeaponToInventoryWeapon: WeaponObject is null for weapon: %s. Cannot add to inventory."), *Weapon->GetName());

		return false; // Invalid weapon object
	}
	if (this->InventoryWeapons.Num() >= this->MaxWeaponsCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddWeaponToInventoryWeapon: Cannot add weapon: %s to inventory. Maximum weapons count reached."), *WeaponObject->GetName());
		return false;
	}
	Weapon->AttachToActor(this->GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	this->SpawnedWeapons.Add(Weapon); // Add the spawned weapon to the array
	this->InventoryWeapons.Add(WeaponObject);
	UE_LOG(LogTemp, Log, TEXT("AddWeaponToInventoryWeapon: Added weapon: %s to inventory."), *WeaponObject->GetName());
	/* Don't destroy the weapon as it's only attached to the character */
	return true;
}

bool UInventoryComponent::AddAmmoToInventory(AAmmo* Ammo)
{
	if (Ammo == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AddAmmoToInventory: Cannot add null ammo to inventory."));
		return false;
	}
	if (!IsValid(Ammo->ItemObject)) {
		UE_LOG(LogTemp, Error, TEXT("AddAmmoToInventory: AddWeaponToInventoryWeapon: Invalid ItemObject. Cannot initialize animations."));
		return false; // Invalid item object
	}
	UAmmoObject* AmmoObject = Cast<UAmmoObject>(Ammo->ItemObject);
	if (!IsValid(AmmoObject))
	{
		UE_LOG(LogTemp, Error, TEXT("AddAmmoToInventory: AmmoObject is null for Ammo: %s. Cannot add to inventory."), *Ammo->GetName());

		return false; // Invalid weapon object
	}
	if (this->InventoryAmmo.Num() >= this->MaxAmmosCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddAmmoToInventory: Cannot add ammo: %s to inventory. Maximum ammo count reached."), *AmmoObject->GetName());
		return false;
	}
	this->InventoryAmmo.Add(AmmoObject);
	UE_LOG(LogTemp, Log, TEXT("AddAmmoToInventory: Added ammo: %s to inventory."), *AmmoObject->GetName());
	Ammo->Destroy(); // Destroy the ammo actor after adding to inventory
	return true;
}

bool UInventoryComponent::AddItemToInventory(AItem* Item)
{
	if (Item == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot add null item to inventory."));
		return false;
	}


	if(Cast<AWeapon>(Item))
	{
		AWeapon* Weapon = Cast<AWeapon>(Item);
		return this->AddWeaponToInventoryWeapon(Weapon);
	} else if (Cast<AAmmo>(Item))
	{
		UAmmoObject* AmmoObject = Cast<UAmmoObject>(Item);
		
	}
	UItemObject* ItemObject = Item->ItemObject;
	if(ItemObject == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemObject is null for item: %s. Cannot add to inventory."), *Item->GetName());
		return false;
	}
	if (this->InventoryItems.Num() >= this->MaxItemsCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot add item: %s to inventory. Maximum items count reached."), *ItemObject->GetName());
		return false;
	}
	this->InventoryItems.Add(ItemObject);
	UE_LOG(LogTemp, Log, TEXT("Added item: %s to inventory."), *ItemObject->GetName());
	Item->Destroy(); // Destroy the item actor after adding to inventory
	return true;
}

