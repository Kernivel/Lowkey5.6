


#include "Item/Weapon/ActorWeapon.h"


// Sets default values for this component's properties
AActorWeapon::AActorWeapon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;
	FirstPersonWeaponSkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonWeaponSkelMesh"));
	ThirdPersonWeaponSkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonWeaponSkelMesh"));
	FirstPersonWeaponSkelMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	ThirdPersonWeaponSkelMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;
}


// Called when the game starts
void AActorWeapon::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void AActorWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}