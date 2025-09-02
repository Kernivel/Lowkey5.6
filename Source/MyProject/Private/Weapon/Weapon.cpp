// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"

AWeapon::AWeapon(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.DoNotCreateDefaultSubobject(TEXT("ItemObject")))
{
	ItemObject = CreateDefaultSubobject<UWeaponObject>(TEXT("WeaponObject")); // Initialize ItemObject
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh; // Set the root component to the weapon mesh
	CollisionSphere->SetupAttachment(RootComponent); // Attach the collision sphere to the root component
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Initialize the weapon object after components are initialized
	AWeapon::CreateWeaponSubobject();
}

UWeaponObject* AWeapon::GetWeaponObject()
{
	if (IsValid(this->ItemObject))
	{
		return Cast<UWeaponObject>(this->ItemObject);
	}
	UE_LOG(LogTemp, Error, TEXT("GetWeaponObject: ItemObject is not valid. Cannot get weapon object."));
	return nullptr; // Return nullptr if the item object is not valid
}

void AWeapon::Initialize(UWeaponObject *InWeaponObject)
{
	this->ItemObject = InWeaponObject;
	if (this->bIsFirstPersonView) {
		/* If the bIsFirstPersonView is set, then the weapon is the hand of a player, and should not have collisions */
		this->CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Disable collision for the weapon item
		this->WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Disable collision for the weapon mesh
	}
	else 
	{
		this->CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		this->WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	//this->CollisionSphere->AttachToComponent(this->WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	this->bCanBePickedUp = !this->bIsFirstPersonView;
	this->bIsPickedUp = this->bIsFirstPersonView;
	AWeapon::InitMesh(this->bIsFirstPersonView);
	AWeapon::InitializeAnimationData();
}

void AWeapon::CreateWeaponSubobject()
{
	this->ItemObject = NewObject<UWeaponObject>(this, UWeaponObject::StaticClass());
}

bool AWeapon::InitMesh(bool FirstPersonView)
{
	UWeaponObject* WeaponObject = this->GetWeaponObject();
	if(!WeaponObject)
	{
		UE_LOG(LogTemp, Error, TEXT("InitMesh: Invalid WeaponObject, cannot retreive the corresponding Weapon Mesh to initialize"));
		return false; // Invalid weapon object
	}
	TSoftObjectPtr<USkeletalMesh> SoftSkelMeshReference = WeaponObject->SoftSkelMeshReference;
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	TSharedPtr<FStreamableHandle> Handle = Streamable.RequestAsyncLoad(
		SoftSkelMeshReference.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this, SoftSkelMeshReference, FirstPersonView]()
			{
				if(!IsValid(this) || SoftSkelMeshReference == nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("Weapon or SoftSkelMeshReference is null during async load."));
					return; // Early exit if the weapon or reference is invalid
				}
				USkeletalMesh* LoadedMesh = SoftSkelMeshReference.Get();
				if (IsValid(LoadedMesh) && IsValid(this->WeaponMesh))
				{
					if (FirstPersonView) {
						this->WeaponMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson; // Set the weapon mesh to first person type
					}
					// Use your loaded mesh, e.g.:
					this->WeaponMesh->SetSkeletalMesh(LoadedMesh);
				}else{
					UE_LOG(LogTemp, Error, TEXT("Failed to load the skeletal mesh from the provided reference."));
				}
			})
	);
	if(!Handle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to request async load for the skeletal mesh."));
		return false; // Failed to request async load
	}

	if (this->WeaponMesh!=nullptr) {
		return true;
	}
	UE_LOG(LogTemp, Error, TEXT("No Weapon Mesh"));
	return false; // Failed to initialize the mesh
}

bool AWeapon::InitializeAnimationData()
{
	UE_LOG(LogTemp, Log, TEXT("InitializeAnimationData: Starting loading weapon animations"));
	UWeaponObject* WeaponObject = this->GetWeaponObject();
	if(!WeaponObject)
	{
		UE_LOG(LogTemp, Error, TEXT("InitializeAnimationData: Invalid WeaponObject."));
		return false; // Invalid weapon object
	}
	if(WeaponObject->AnimBPClass.IsValid() || WeaponObject->AnimBPClass.ToSoftObjectPath().IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("InitializeAnimationData: Animation asset is valid loading weapon animations"));
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		TSharedPtr<FStreamableHandle> Handle = Streamable.RequestAsyncLoad(
			WeaponObject->AnimBPClass.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda([this, WeaponObject]()
				{
					if(!IsValid(this) || !IsValid(WeaponObject))
					{
						UE_LOG(LogTemp, Error, TEXT("InitializeAnimationData: Weapon or WeaponObject is null during async load."));
						return; // Early exit if the weapon or object is invalid
					}
					UClass* LoadedAnimBPClass = WeaponObject->AnimBPClass.Get();
					if(!LoadedAnimBPClass)
					{
						UE_LOG(LogTemp, Error, TEXT("InitializeAnimationData: Failed to load the Animation Blueprint Class from the provided reference."));
						return; // Early exit if the animation blueprint class is invalid
					}
					if (LoadedAnimBPClass && this->WeaponMesh)
					{
						this->WeaponMesh->SetAnimInstanceClass(LoadedAnimBPClass);
					}
				})
		);
		if (!Handle.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("InitializeAnimationData: Failed to request async load for the skeletal mesh."));
			return false; // Failed to request async load
		}
		UE_LOG(LogTemp, Log, TEXT("InitializeAnimationData: Async loaded this Weapon animation"));
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InitializeAnimationData: Invalid Animation Blueprint Class for WeaponObject."));
		return false;
	}
}

FTransform AWeapon::GetMuzzleWorldTransform() const
{
	if (this->WeaponMesh)
	{
		return this->WeaponMesh->GetSocketTransform("MuzzleSocket", ERelativeTransformSpace::RTS_World);
	}
	UE_LOG(LogTemp, Error, TEXT("GetMuzzleWorldTransform: Weapon mesh is not valid. Cannot get muzzle transform."));
	return FTransform::Identity; // Return an identity transform if the mesh is not valid
}

bool AWeapon::Fire()
{
	UWeaponObject* WeaponObject = this->GetWeaponObject();
	if(!WeaponObject)
	{
		UE_LOG(LogTemp, Error, TEXT("Fire: Invalid WeaponObject."));
		return false; // Invalid weapon object
	}
	if(WeaponObject->CurrentAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire: No ammo left to fire!"));
		return false; // No ammo left
	}
	if(this->bIsFiring)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire: Weapon is already firing!"));
		return false; // Already firing
	}
	this->bIsFiring = true;
	GetWorldTimerManager().SetTimer(
		this->FireTimerHandle, 
		this, 
		&AWeapon::ResetFireState,
		WeaponObject->DelayBetweenShots, 
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
	OnWeaponFire.Broadcast(); // Broadcast the OnFire event
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
	UWeaponObject* WeaponObject = this->GetWeaponObject();
	if (!IsValid(WeaponObject))
	{
		UE_LOG(LogTemp, Error, TEXT("Fire: Invalid WeaponObject."));
		return false; // Invalid weapon object
	}

	// Initialize main and optional accessories if needed
	if (WeaponObject)
	{
		// Example: Load main accessories
		for (const auto& AccessoryPair : WeaponObject->MainAccessories)
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