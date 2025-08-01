// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"

AWeapon::AWeapon()
{
	Super::BeginPlay();
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh; // Set the root component to the weapon mesh
}

bool AWeapon::Initialize(UWeaponObject* InWeaponObject, bool FirstPersonView)
{
	if (!InWeaponObject)
	{
		return false; // Invalid weapon object
	}
	this->WeaponObject = InWeaponObject;
	AWeapon::InitMesh(FirstPersonView);
	AWeapon::InitializeAnimationData();
	return true;
}

bool AWeapon::InitMesh(bool FirstPersonView)
{
	if(!IsValid(WeaponObject))
	{
		return false; // Invalid weapon object
	}
	TSoftObjectPtr<USkeletalMesh> SoftSkelMeshReference = WeaponObject->SoftSkelMeshReference;
	if(SoftSkelMeshReference.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("SoftSkelMeshReference is null! Cannot initialize weapon mesh."));
		return false; // Failed to initialize the mesh
	}
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	Streamable.RequestAsyncLoad(
		SoftSkelMeshReference.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this, SoftSkelMeshReference, FirstPersonView]()
			{
				USkeletalMesh* LoadedMesh = SoftSkelMeshReference.Get();
				if (LoadedMesh && IsValid(WeaponMesh))
				{
					if (FirstPersonView) {
						WeaponMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson; // Set the weapon mesh to first person type
					}
					// Use your loaded mesh, e.g.:
					WeaponMesh->SetSkeletalMesh(LoadedMesh);
				}else{
					UE_LOG(LogTemp, Error, TEXT("Failed to load the skeletal mesh from the provided reference."));
				}
			})
	);

	if (this->WeaponMesh!=nullptr) {
		return true;
	}
	UE_LOG(LogTemp, Error, TEXT("No Weapon Mesh"));
	return false; // Failed to initialize the mesh
}

bool AWeapon::InitializeAnimationData()
{
	UE_LOG(LogTemp, Log, TEXT("Starting loading weapon animations"));
	if(this->WeaponObject->AnimBPClass.IsValid() || this->WeaponObject->AnimBPClass.ToSoftObjectPath().IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Animation asset is valid loading weapon animations"));
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		Streamable.RequestAsyncLoad(
			this->WeaponObject->AnimBPClass.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda([this]()
				{
					UClass* LoadedAnimBPClass = this->WeaponObject->AnimBPClass.Get();
					if (LoadedAnimBPClass && this->WeaponMesh)
					{
						this->WeaponMesh->SetAnimInstanceClass(LoadedAnimBPClass);
					}
				})
		);
		UE_LOG(LogTemp, Log, TEXT("Async loaded this Weapon animation"));
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Animation Blueprint Class for WeaponObject."));
		return false;
	}
}

FTransform AWeapon::GetMuzzleWorldTransform() const
{
	if (this->WeaponMesh)
	{
		return this->WeaponMesh->GetSocketTransform("MuzzleSocket", ERelativeTransformSpace::RTS_World);
	}
	UE_LOG(LogTemp, Error, TEXT("Weapon mesh is not valid. Cannot get muzzle transform."));
	return FTransform::Identity; // Return an identity transform if the mesh is not valid
}

bool AWeapon::Fire()
{
	if(!this->WeaponObject)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponObject is not initialized. Cannot fire."));
		return false; // Weapon object is not initialized
	}
	if(this->WeaponObject->CurrentAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ammo left to fire!"));
		return false; // No ammo left
	}
	if(this->bIsFiring)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon is already firing!"));
		return false; // Already firing
	}
	this->bIsFiring = true;
	GetWorldTimerManager().SetTimer(
		this->FireTimerHandle, 
		this, 
		&AWeapon::ResetFireState,
		this->WeaponObject->DelayBetweenShots, 
		false
	);
	if(MuzzleFlash)
	{
		// Spawn the muzzle flash effect at the muzzle location
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this->GetWorld(),
			MuzzleFlash,
			this->GetMuzzleWorldTransform().GetLocation(),
			this->GetMuzzleWorldTransform().GetRotation().Rotator(),
			FVector(1.f), // Scale
			true // Auto destroy the system after completion
		);
	}
	return true;
}

void AWeapon::ResetFireState()
{
	// Reset the firing state after the delay
	this->bIsFiring = false;
	UE_LOG(LogTemp, Log, TEXT("Weapon fire state reset."));
}

bool AWeapon::InitializeAccessories()
{
	// Initialize main and optional accessories if needed
	if (this->WeaponObject)
	{
		// Example: Load main accessories
		for (const auto& AccessoryPair : this->WeaponObject->MainAccessories)
		{
			const FString& AccessoryName = AccessoryPair.Key;
			const UAccessoryObject* AccessoryObject = AccessoryPair.Value;
			if (AccessoryObject)
			{
				// Load or apply the accessory to the weapon
				UE_LOG(LogTemp, Log, TEXT("Loaded main accessory: %s"), *AccessoryName);
				TSoftObjectPtr<UStaticMesh> MeshReference = AccessoryObject->SoftStaticMeshReference;
				FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

				Streamable.RequestAsyncLoad(
					MeshReference.ToSoftObjectPath(),
					FStreamableDelegate::CreateLambda([this, MeshReference]()
						{
							UStaticMesh* LoadedMesh = MeshReference.Get();
							if (LoadedMesh && IsValid(WeaponMesh))
							{
								// Use your loaded mesh, e.g.:
								//WeaponMesh->SetSkeletalMesh(LoadedMesh);
							}
							else {
								UE_LOG(LogTemp, Error, TEXT("Failed to load the skeletal mesh from the provided reference."));
							}
						})
				);
			}
		}
		
		return true; // Successfully initialized accessories
	}
	return false; // Failed to initialize accessories
}