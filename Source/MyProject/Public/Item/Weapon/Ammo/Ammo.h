#pragma once
#include "Item/Item.h"
#include "Item/Weapon/Ammo/AmmoObject.h"
#include "Ammo.generated.h"

UCLASS(Blueprintable)

class AAmmo :
    public AItem
{
	GENERATED_BODY()
public:
    // Sets default values for this actor's properties
    AAmmo(const FObjectInitializer &ObjectInitializer);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* AmmoMesh; // Mesh component for the ammo
};

