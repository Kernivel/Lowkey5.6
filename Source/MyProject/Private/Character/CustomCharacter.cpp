// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomCharacter.h"

/*******************************************
*Constructor & Construct helpers functions * 
********************************************/

// Sets default values
ACustomCharacter::ACustomCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ACustomCharacter::ConstructDefaultMetahuman();
	this->Inventory=CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	/* Add all the sockets necessary to attach weapons */
	ACustomCharacter::PopulateWeaponSocketMap();
	GetCharacterMovement()->GravityScale = 2.5f; // Set the default gravity scale
	GetCharacterMovement()->MaxWalkSpeed = this->RunSpeed; // Set the default run speed

	TrajectoryComponent = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("TrajectoryComponent"));
	
	GetCharacterMovement()->MaxWalkSpeedCrouched = this->CrouchSpeed; // Enable crouching for the character
	
	this->ConcreteImpactEffect = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/Niagara/Blueprints/NS_ConcreteImapact"));

}
/* METAHUMAN SETUP */
void ACustomCharacter::ConstructDefaultMetahuman()
{
	// Create the skeletal mesh components for the character's body parts
	TorsoMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TorsoMesh"));
	PantsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PantsMesh"));
	ShoesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShoesMesh"));
	GlovesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GlovesMesh"));
	FaceMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HairMesh = CreateDefaultSubobject<UGroomComponent>(TEXT("HairMesh"));

	// Attach the meshes to the body component
	TorsoMesh->SetupAttachment(GetMesh());
	PantsMesh->SetupAttachment(GetMesh());
	FaceMesh->SetupAttachment(GetMesh());
	ShoesMesh->SetupAttachment(GetMesh());

	GlovesMesh->SetupAttachment(GetMesh());
	// Set the master poses for the meshes
	TorsoMesh->SetLeaderPoseComponent(GetMesh());
	PantsMesh->SetLeaderPoseComponent(GetMesh());
	ShoesMesh->SetLeaderPoseComponent(GetMesh());
	GlovesMesh->SetLeaderPoseComponent(GetMesh());
	FaceMesh->SetLeaderPoseComponent(GetMesh());
	// Load the default meshes for the character from Daniel Metahuman
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshAsset(TEXT("/Game/MetaHumans/Daniel/Body/m_med_nrw_body"));
	if(BodyMeshAsset.Succeeded())
	{
		// Loads naked body 
		GetMesh()->SetSkeletalMesh(BodyMeshAsset.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BodyMeshAsset not found!"));
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FaceMeshAsset(TEXT("/Game/MetaHumans/Daniel/Face/Daniel_FaceMesh"));
	if ( FaceMeshAsset.Succeeded())
	{
		// Loads naked face 
		FaceMesh->SetSkeletalMesh(FaceMeshAsset.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FaceMeshAsset not found!"));
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TorsoMeshAsset(TEXT("/Game/MetaHumans/Common/Male/Medium/NormalWeight/Tops/Crewneckt/m_med_nrw_top_crewneckt_nrm_Medium"));
	if (TorsoMeshAsset.Succeeded())
	{
		// Loads shirt wear
		TorsoMesh->SetSkeletalMesh(TorsoMeshAsset.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TorsoMeshAsset not found!"));
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PantsMeshAsset(TEXT("/Game/MetaHumans/Common/Male/Medium/NormalWeight/Bottoms/Jeans/m_med_nrw_btm_jeans_nrm_Medium"));
	if(PantsMeshAsset.Succeeded())
	{
		// Loads pants wear
		PantsMesh->SetSkeletalMesh(PantsMeshAsset.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PantsMeshAsset not found!"));
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ShoesMeshAsset(TEXT("/Game/MetaHumans/Common/Male/Medium/NormalWeight/Shoes/Boots/m_med_nrw_shs_boots_Medium"));
	if(ShoesMeshAsset.Succeeded())
	{
		// Loads shoes wear
		ShoesMesh->SetSkeletalMesh(ShoesMeshAsset.Object);
	}
	else
		{
		UE_LOG(LogTemp, Warning, TEXT("FeetMeshAsset not found!"));
	}


	/* Move the meshes to be at the correct height*/
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}

void ACustomCharacter::PopulateWeaponSocketMap()
{
	// Populate the WeaponSocketMap with predefined socket names and their corresponding weapon objects
	ACustomCharacter::WeaponSocketMap.Add(EWeaponType::Pistol, FName("PistolSocket"));
	ACustomCharacter::WeaponSocketMap.Add(EWeaponType::AssaultRifle, FName("BackSocket"));
	ACustomCharacter::WeaponSocketMap.Add(EWeaponType::SubmachineGun, FName("BackSocket"));
	ACustomCharacter::WeaponSocketMap.Add(EWeaponType::None, FName("BackSocket"));
	// Add more sockets as needed
}

/*******************************************
* Unreal Base Functions                    *
********************************************/

// Called when the game starts or when spawned
void ACustomCharacter::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Custom Character was created"));
}

// Called every frame
void ACustomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACustomCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


/*******************************************
* Inventory Functions                      *
********************************************/

/*
* Function to switch to a specific weapon by its index in the inventory.
* Inventory holds the list of weapons and the currenyly equipped weapon.
*/
bool ACustomCharacter::SwitchToWeapon(int WeaponIndex)
{	
	UWeaponObject* WeaponObject = Inventory->SwitchWeaponIndex(WeaponIndex);
	if (WeaponObject != nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Switched to weapon index: ") + FString::FromInt(WeaponIndex));
		FString WeaponNameMessage = Inventory->SwitchWeaponIndex(WeaponIndex)->GetName() + TEXT( ":equiped!");
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, WeaponNameMessage);
		return true;
	}
	// If the weapon index is invalid or the weapon is not found, log an error message
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to switch weapon. Invalid index or weapon not found."));
	return false;
}

void ACustomCharacter::InstanciateAllWeaponItems()
{
	this->Inventory->SpawnWeaponsFromInventory(this,false);
}


void ACustomCharacter::InstanciateWeaponItem(UWeaponObject* Weapon){
	this->Inventory->SpawnWeaponItem(this, Weapon, false);
}


/*******************************************
* Socket Functions                         *
********************************************/

void ACustomCharacter::AttachWeaponsToSockets()
{
	UE_LOG(LogTemp, Log, TEXT("ACu Attaching weapons to sockets..."));
	for(AWeapon* ChildWeapon : this->Inventory->SpawnedWeapons){
		UE_LOG(LogTemp, Log, TEXT("ACu Attaching weapon: %s"), *ChildWeapon->GetName());
		EWeaponType WeaponType = ChildWeapon->GetWeaponObject()->GetWeaponType(); // Get the socket name from the weapon item
		/* If there are no matching socket, the wepaon will be attached to the back */
		FName SocketName = FName(TEXT("BackSocket"));

		/* Get a specific socket if needed (holster...) */
		if (ACustomCharacter::WeaponSocketMap.Contains(WeaponType))
		{
				SocketName = ACustomCharacter::WeaponSocketMap[WeaponType]; // Get the socket name from the map
				UE_LOG(LogTemp, Log, TEXT("ACu Using socket: %s for weapon type: %s"), *SocketName.ToString(), *UEnum::GetValueAsString(WeaponType));
		}


		/* The current child weapon is the equiped weapon so we need to spawn it in the hand */
		/* Check for OOB first */
		if(this->Inventory->CurrentlyEquipedIndex < this->Inventory->InventoryWeapons.Num() && ChildWeapon->GetWeaponObject() == this->Inventory->InventoryWeapons[this->Inventory->CurrentlyEquipedIndex])
		{
			SocketName = FName(TEXT("RightHandSocket")); // Attach the current weapon to the right hand socket
			UE_LOG(LogTemp, Log, TEXT("ACu Override Equiped weapon: %s to socket: %s"), *ChildWeapon->GetName(), *SocketName.ToString());
		}
		ACustomCharacter::AttachWeaponToSocket(ChildWeapon, SocketName);
	}
}

void ACustomCharacter::AttachWeaponToSocket(AWeapon* Weapon, FName SocketName)
{
	UE_LOG(LogTemp, Log, TEXT("ACu Attaching weapon: %s to socket: %s"), *Weapon->GetName(), *SocketName.ToString());
	if (Weapon && Weapon->GetWeaponMesh())
	{
		/* First Remove the weapon */
		const FDetachmentTransformRules DetachRules = FDetachmentTransformRules::KeepWorldTransform;
		Weapon->GetWeaponMesh()->DetachFromComponent(DetachRules);
		if (!SocketName.IsNone())
		{
			// Attach the weapon mesh to the specified socket on the character's mesh
			if (Weapon->GetWeaponMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName)) {
				UE_LOG(LogTemp, Log, TEXT("Weapon %s attached to socket %s successfully."), *Weapon->GetName(), *SocketName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Weapon attached successfully!"));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to attach weapon to socket."));
				UE_LOG(LogTemp, Warning, TEXT("Failed to attach weapon %s to socket %s."), *Weapon->GetName(), *SocketName.ToString());
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to attach weapon. Invalid socket name."));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to attach weapon. Invalid weapon or mesh."));
	}
}

/*******************************************
* Action Functions                         *
********************************************/

void ACustomCharacter::Fire()
{
	//empty function to be overriden by the child class
	UE_LOG(LogTemp, Log, TEXT("ACu Fire function called, but not implemented in base class."));
}

bool ACustomCharacter::CanReload() const {
	// Check if the current weapon is valid and has ammo to reload
	if (this->IsCurrentWeaponValid())
	{
		AWeapon* CurrentWeapon = this->Inventory->SpawnedWeapons[this->Inventory->CurrentlyEquipedIndex];
		/* Check if Current Ammo is under the maximum ammo the weapon can hold */
		if (CurrentWeapon && CurrentWeapon->GetWeaponObject() && CurrentWeapon->GetWeaponObject()->CurrentAmmo < CurrentWeapon->GetWeaponObject()->AmmoCapacity)
		{
			if(this->Inventory->GetAmmoOfType(CurrentWeapon->GetWeaponObject()->GetWeaponType()) <= 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No ammo of the correct type found in inventory."));
				return false; // Cannot reload if no ammo of the correct type is available
			}
			return true; // Can reload if the weapon has less ammo than max
		}
	}
	return false; // Cannot reload if the weapon is invalid or already full
}

uint8 ACustomCharacter::Reload()
{
	uint8 ReloadedAmmount = 0;
	if (!this->CanReload())
	{
		return 0;
	}
	UWeaponObject* CurrentWeaponObject = this->Inventory->SpawnedWeapons[this->Inventory->CurrentlyEquipedIndex]->GetWeaponObject();
	if(CurrentWeaponObject == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Current weapon object is null. Cannot reload."));
		return 0; // Cannot reload if the current weapon object is null
	}
	if(CurrentWeaponObject->Magazine == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Current weapon has no magazine. Cannot reload."));
		return 0; // Cannot reload if the current weapon has no magazine
	}
	// Get the current ammo type
	EWeaponType WeaponType = CurrentWeaponObject->GetWeaponType();
	uint8 CurrentAmmo = CurrentWeaponObject->CurrentAmmo; // Get the current ammo in the weapon
	uint8 AmmoToReload = CurrentWeaponObject->Magazine->MagazineSize - CurrentAmmo; // Calculate how much ammo we need to reload
	this->Inventory->RetreiveAmmoOfType(WeaponType, AmmoToReload);
	return ReloadedAmmount;
}

/*******************************************
* Movement Functions                       *
********************************************/
void ACustomCharacter::BeginSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = this->SprintSpeed; 
}

void ACustomCharacter::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = this->RunSpeed; // Double the walk speed for sprinting
}

void ACustomCharacter::BeginWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = this->WalkSpeed;
}

void ACustomCharacter::EndWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = this->RunSpeed; // Double the walk speed for sprinting
}

/**********************
* Roation functions   *
***********************/

void ACustomCharacter::UpdateRotatorVariables()
{
	// Update the previous and current rotation variables
	this->PreviousRotation = this->CurrentRotation;
	this->CurrentRotation = GetActorRotation();
}

FRotator ACustomCharacter::GetRotationDelta(FRotator PrevRotation, FRotator CurrRotation)
{
	return CurrRotation - PrevRotation;

}

FRotator ACustomCharacter::GetRotationSpeedFromRotationDelta(FRotator RotationDelta, float DeltaTime)
{
	if (FMath::IsNearlyZero(DeltaTime))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DeltaTime is nearly zero, returning zero rotation speed."));
		return FRotator::ZeroRotator;
	}
	return FRotator(
			RotationDelta.Pitch / DeltaTime,
			RotationDelta.Yaw / DeltaTime,
			RotationDelta.Roll / DeltaTime
		);
}

FRotator ACustomCharacter::GetRotationSpeed()
{
	FRotator RotationDelta = ACustomCharacter::GetRotationDelta(this->PreviousRotation, this->CurrentRotation);
	return GetRotationSpeedFromRotationDelta(RotationDelta, GetWorld()->GetDeltaSeconds());
}

bool ACustomCharacter::IsCurrentWeaponValid() const
{
	// Check proper initialization of the Inventory component
	if(this->Inventory == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is null. Cannot check current weapon validity."));
		return false; // Inventory is not initialized
	}
	if(!this->Inventory->InventoryWeapons.IsValidIndex(this->Inventory->CurrentlyEquipedIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentlyEquipedIndex is out of bounds. Cannot check current weapon validity."));
		return false; // Index is out of bounds
	}
	if (this->Inventory->SpawnedWeapons.IsValidIndex(this->Inventory->CurrentlyEquipedIndex))
	{
		AWeapon* CurrentWeapon = this->Inventory->SpawnedWeapons[this->Inventory->CurrentlyEquipedIndex];
		if(CurrentWeapon && CurrentWeapon->GetWeaponObject())
		{
			return true; // The current weapon is valid
		}
	}
	return false; // The current weapon is not valid or the index is out of bounds
}