#pragma once
#include "Item.h"
#include "AmmoObject.h"
#include "Ammo.generated.h"

UCLASS(Blueprintable)

class AAmmo :
    public AItem
{
	GENERATED_BODY()
public:
    // Sets default values for this actor's properties
    AAmmo();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* AmmoMesh; // Mesh component for the ammo
};

