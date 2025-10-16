


#include "Character/WeaponComponent.h"


// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool  UWeaponComponent::Fire()
{
	if (this->bIsFiring)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire: Weapon is already firing!"));
		return false; // Already firing
	}
	this->bIsFiring = true;
	this->GetOwner()->GetWorldTimerManager().SetTimer(
		this->FireTimerHandle,
		this,
		&UWeaponComponent::ResetFireState,
		this->DelayBetweenShots,
		false
	);
	return true;
}

void UWeaponComponent::ResetFireState()
{
	this->bIsFiring = false;
}