

#pragma once

#include "CoreMinimal.h"
#include "Item/ActorItem.h"
#include "ActorWeapon.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API AActorWeapon : public AActorItem
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AActorWeapon();

	UPROPERTY(BlueprintReadWrite)
	USkeletalMeshComponent* FirstPersonWeaponSkelMesh;
	UPROPERTY(BlueprintReadWrite)
	USkeletalMeshComponent* ThirdPersonWeaponSkelMesh;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

		
	
};
