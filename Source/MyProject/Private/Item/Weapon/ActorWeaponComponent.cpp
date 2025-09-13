


#include "Item/Weapon/ActorWeaponComponent.h"


// Sets default values for this component's properties
UActorWeaponComponent::UActorWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	FirstPersonWeaponSkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonWeaponSkelMesh"));
}


// Called when the game starts
void UActorWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActorWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


