#include "Weapon/Ammo/Ammo.h"

AAmmo::AAmmo(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.DoNotCreateDefaultSubobject(TEXT("ItemObject")))
{
	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
	ItemObject = CreateDefaultSubobject<UAmmoObject>(TEXT("WeaponObject"));
}