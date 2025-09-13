#include "Item/Weapon/Ammo/AmmoObject.h"

bool UAmmoObject::InitializeData(const FAmmoData& AmmoData)
{
	/* Set the values for ItemObject first */
	this->SetItemObjectName(AmmoData.Name);
	this->SetItemObjectDescription(AmmoData.Description);
	this->SetItemObjectImage(AmmoData.Image);

	/* Set the Weapon specific values */
	this->SoftStaticMeshReference = AmmoData.StaticMesh;
	this->WeaponType = AmmoData.WeaponType;
	/* Set the Weapon specific stats */
	return true;
}

void UAmmoObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UAmmoObject, AmmoCount);
	DOREPLIFETIME(UAmmoObject, WeaponType);
}

void UAmmoObject::OnRep_AmmoCount()
{
	UE_LOG(LogTemp, Log, TEXT("AmmoCount replicated: %d"), AmmoCount);
}

void UAmmoObject::OnRep_WeaponType()
{
	UE_LOG(LogTemp, Log, TEXT("WeaponType replicated: %s"), *UEnum::GetValueAsString(WeaponType));
}
