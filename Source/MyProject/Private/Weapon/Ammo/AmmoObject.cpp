#include "Weapon/Ammo/AmmoObject.h"

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