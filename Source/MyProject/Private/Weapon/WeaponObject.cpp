// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponObject.h"

bool UWeaponObject::InitializeData(const FWeaponData& WeaponData)
{
	/* Set the values for ItemObject first */
	this->SetItemObjectName(WeaponData.Name);
	this->SetItemObjectDescription(WeaponData.Description);
	this->SetItemObjectImage(WeaponData.Image);

	/* Set the Weapon specific values */
	this->SoftSkelMeshReference = WeaponData.SkeletalMesh;
	this->Type = WeaponData.Type;
	/* Set the Weapon specific stats */
	this->BaseDamage = WeaponData.Damage;
	this->BaseFireRate = WeaponData.FireRate;
	this->BaseRange = WeaponData.Range;
	this->AmmoCapacity = WeaponData.AmmoCapacity;
	this->CurrentAmmo = WeaponData.AmmoCapacity; // Initialize current ammo to the maximum capacity
	this->DelayBetweenShots = CalculateDelayBetweenShots(this->BaseFireRate); // Calculate the delay between shots based on the fire rate
	return true;
}

bool UWeaponObject::InitializeAnimationData(const FWeaponAnimation& AnimationData)
{
	this->AnimBPClass = AnimationData.AnimBPClass;
	return true;
}

