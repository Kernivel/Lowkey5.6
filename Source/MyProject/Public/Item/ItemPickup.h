

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h" // Ajoutez cette ligne en haut du fichier
#include "GameFramework/Actor.h"
#include "ItemPickup.generated.h"

UCLASS()
class MYPROJECT_API AItemPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemPickup(const FObjectInitializer& ObjectInitialize);

	void  InitCollisionSphere();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	USphereComponent* CollisionSphere;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
