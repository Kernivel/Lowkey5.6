// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayableCharacter.h"
#include "AbilitySystem/CustomAbilitySystemComponent.h"
#include "Player/CustomPlayerState.h"

/* Constructor and construct functions */
// Constructor
APlayableCharacter::APlayableCharacter()
{
	// Fetch input character mapping
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingObject(TEXT("/Game/FirstPerson/Input/IMC_Default"));
	if(InputMappingObject.Succeeded())
	{
		InputMapping = InputMappingObject.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load InputMappingContext!"));
		InputMapping = nullptr;
	}
	// Load Input Actions from helper function
	APlayableCharacter::ConstructorLoadInputActions();

	/* Add Camera */
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);
	CameraComponent->bUsePawnControlRotation = true; // Allow camera to rotate with the player
	// Load the first person mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ArmsMesh(TEXT("/Game/MetaHumans/Daniel/Body/m_med_nrw_body"));
	if(ArmsMesh.Succeeded())
	{
		FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
		FirstPersonMesh->SetSkeletalMesh(ArmsMesh.Object);
		/* Attach FPS mesh to camera */
		FirstPersonMesh->SetupAttachment(CameraComponent);
		FirstPersonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load First Person Mesh!"));
	}
	FirstPersonMesh->SetRelativeLocation(FVector(5.f, 0.f, -145.f)); // Position the mesh below the camera
	FirstPersonMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f)); // Set the mesh rotation to match the camera

	/* Init extra fps meshes */
	FirstPersonGlovesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonGlovesMesh"));
	FirstPersonGlovesMesh->SetupAttachment(CameraComponent);
	FirstPersonGlovesMesh->SetLeaderPoseComponent(FirstPersonMesh);
	FirstPersonGlovesMesh->SetRelativeLocation(FirstPersonMesh->GetRelativeLocation());
	FirstPersonGlovesMesh->SetRelativeRotation(FirstPersonMesh->GetRelativeRotation());
}

// Don't use outside of the constructor!
void APlayableCharacter::ConstructorLoadInputActions()
{
	UE_LOG(LogTemp, Display, TEXT("Loading Input Actions..."));
	static ConstructorHelpers::FObjectFinder<UInputAction> ShootAction(TEXT("/Game/FirstPerson/Input/Actions/IA_Shoot"));
	if (ShootAction.Succeeded())
	{
		Shoot = ShootAction.Object;
		UE_LOG(LogTemp, Display, TEXT("Added Shoot Action!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load InputAction for Shoot!"));
		Shoot = nullptr;
	}
}

/* Helper functions */
void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		APlayableCharacter::SetupPlayerInputCallBacks();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InputComponent is not an EnhancedInputComponent in Player Character!"));
	}
}



void APlayableCharacter::SetupPlayerInputCallBacks()
{
	if (InputMapping == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("InputMapping is null! Cannot setup input callbacks."));
		return;
	}
	else
	{
		// Bind input actions to functions
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(this->InputComponent))
		{
			EnhancedInputComponent->BindAction(Shoot, ETriggerEvent::Started, this, &APlayableCharacter::Fire);
			UE_LOG(LogTemp, Display, TEXT("Shoot was bind"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("InputComponent is not an EnhancedInputComponent!"));
		}
	}

}


/* Basic Actor functions*/
void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Calls the player controller state which is only available after BeginPlay (not in constructor)	
	APlayableCharacter::GetInputSystem();
}

// VERSION SIMPLE ET PROPRE
void APlayableCharacter::InitAbilitySystemComponent()
{
	/* Delay initialization to the next tick to ensure PlayerState is valid */
	/*  Otherwise might not work for standalone games */
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			if (ACustomPlayerState* CustomPlayerState = GetPlayerState<ACustomPlayerState>())
			{
				AbilitySystemComponent = CastChecked<UCustomAbilitySystemComponent>(CustomPlayerState->GetAbilitySystemComponent());
				AbilitySystemComponent->InitAbilityActorInfo(CustomPlayerState, this);
				AttributeSet = CustomPlayerState->GetAttributeSet();

				// Maintenant initialiser les attributs par défaut
				InitDefaultAttributes();

				UE_LOG(LogTemp, Log, TEXT("GAS initialized successfully!"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("PlayerState still null after NextTick - trying with longer delay"));
				RetryInitAbilitySystem();
			}
		});
}

void APlayableCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilitySystemComponent();
	// NE PAS appeler InitDefaultAttributes() ici - il sera appelé dans le timer
}

// Fonction de fallback au cas où NextTick ne suffit pas
void APlayableCharacter::RetryInitAbilitySystem()
{
	FTimerHandle RetryTimer;
	GetWorld()->GetTimerManager().SetTimer(RetryTimer, [this]()
		{
			if (APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				if (ACustomPlayerState* CustomPlayerState = Cast<ACustomPlayerState>(PC->PlayerState))
				{
					AbilitySystemComponent = CastChecked<UCustomAbilitySystemComponent>(CustomPlayerState->GetAbilitySystemComponent());
					AbilitySystemComponent->InitAbilityActorInfo(CustomPlayerState, this);
					AttributeSet = CustomPlayerState->GetAttributeSet();
					InitDefaultAttributes();
					UE_LOG(LogTemp, Log, TEXT("GAS initialized on retry!"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("PlayerState still null after retry - giving up"));
				}
			}
		}, 0.2f, false);
}

void APlayableCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilitySystemComponent();
	//InitDefaultAttributes();
}

void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* Scan in front of the player for collisions and if actor is a pickable item */
	this->CurrentLookedAtItem = APlayableCharacter::ScanForPickableItems();
	this->LoopOverCurvePlaybacks(DeltaTime);
}

void APlayableCharacter::GetInputSystem()
{
	if(APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if(ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if(UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				InputSubsystem->AddMappingContext(InputMapping, 0);
				UE_LOG(LogTemp, Log, TEXT("Input Mapping Context added successfully."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("EnhancedInputLocalPlayerSubsystem is null!"));
				return;
			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LocalPlayer is null!"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is not a PlayerController!"));
	}
}

void APlayableCharacter::InstanciateAllWeaponItems()
{
	this->Inventory->SpawnWeaponsFromInventory(this, true);
}


void APlayableCharacter::InstanciateWeaponItem(UWeaponObject* Weapon) {
	this->Inventory->SpawnWeaponItem(this, Weapon, true);
}

AItem* APlayableCharacter::ScanForPickableItems()
{
	/* Scan for items in front of the player */
	FHitResult HitResult;
	FVector Start = CameraComponent->GetComponentLocation();
	FVector ForwardVector = CameraComponent->GetForwardVector();
	FVector End = Start + (ForwardVector * 300.f); // Adjust the distance as needed
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // Ignore self
	// Perform the line trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
	{
		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.f, 0, 1.f); // Draw a debug line for visualization
		AActor* HitOwner = HitResult.GetComponent() ? HitResult.GetComponent()->GetOwner() : HitResult.GetActor();
		AItem* HitItem = Cast<AItem>(HitOwner);
		/* Check if HitItem is found and if the conditions to pick it up are complete */
		if (HitItem && HitItem->bCanBePickedUp && !HitItem->bIsPickedUp)
		{
			return HitItem; // Return the first item found
		}
	}
	return nullptr; // No item found
}


void APlayableCharacter::Fire()
{
	/* Call fire function on currently equiped weapon */
	if (!APlayableCharacter::IsCurrentWeaponValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire: No weapons spawned! Cannot fire."));
		return;
	}
	else {
		/* Check for conditions that would stop the character from shooting */
		if (this->bIsRunning || this->bIsReloading) {
			return; // Do not fire while running
		}
		if (!this->Inventory->SpawnedWeapons[this->Inventory->CurrentlyEquipedIndex]->Fire())
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon is not ready to fire!"));
			return; // Weapon is not ready to fire
		}
		else
		{
			/* Do the line trace */
			TArray<FHitResult> HitResultArray;
			FCollisionQueryParams TraceParams(FName(TEXT("DamageTrace")), true, this);
			TraceParams.bTraceComplex = true;
			TraceParams.bReturnPhysicalMaterial = false;
			APlayerController* PC = GetWorld()->GetFirstPlayerController();

			FVector Start = FVector::ZeroVector;
			FVector End = FVector::ZeroVector;
			FVector ForwardVector = FVector::ZeroVector;
			float TraceDistance = 10000.f; // Set the trace distance
			if (PC && PC->PlayerCameraManager)
			{
				FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
				FRotator CameraRotation = PC->PlayerCameraManager->GetCameraRotation();
				ForwardVector = CameraRotation.Vector();

				Start = CameraLocation;
				End = Start + ForwardVector * TraceDistance;

				// Then do your line trace from Start to End
			}
			else 
			{
				/* Do a default line trace with no recoil */
				 Start = CameraComponent->GetComponentLocation();
				 ForwardVector = CameraComponent->GetForwardVector();
				 End = ((ForwardVector * 10000.f) + Start);
			}


			/* LineTraceMultiByChannel returns all the collided actors */
			bool bHit = GetWorld()->LineTraceMultiByChannel(HitResultArray, Start, End, ECC_Visibility, TraceParams);
			if (bHit)
			{
				for (FHitResult HitResult : HitResultArray) {
					AActor* HitActor = HitResult.GetActor();
					FVector HitLocation = HitResult.ImpactPoint;
					FVector HitNormal = HitResult.ImpactNormal;
					FRotator HitRotation = HitResult.ImpactNormal.Rotation();
					//UE_LOG(LogTemp, Warning, TEXT("Hit detected."));
					UWeaponObject* WeaponObject = this->Inventory->SpawnedWeapons[this->Inventory->CurrentlyEquipedIndex]->GetWeaponObject();
					if(!IsValid(WeaponObject))
					{
						UE_LOG(LogTemp, Warning, TEXT("Fire: WeaponObject is null! Cannot apply damage."));
						return; // Invalid weapon object
					}
					float Damage = WeaponObject->BaseDamage;
					UGameplayStatics::ApplyPointDamage(HitActor,Damage,HitNormal, HitResult,GetOwner()->GetInstigatorController(),this,UDamageType::StaticClass());
					FVector BoxExtent = FVector(5.f, 5.f, 5.f); // Small box extent for the debug box
					// Draw a debug box at the hit location
					//DrawDebugBox(GetWorld(), HitLocation, BoxExtent, FColor::Blue, false, 1.f, 0, 1.f);
					if(this->ConcreteImpactEffect != nullptr)
					{
						// Spawn the Niagara effect at the hit location
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), this->ConcreteImpactEffect, HitLocation, HitRotation);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Fire: ConcreteImpactEffect is null! Cannot spawn effect."));
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Fire: No hit detected."));
			}
			//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f, 0, 1.f);
			// Play the camera shake
			ACustomCharacter::OnFire.Broadcast(); // Broadcast the OnFire 
			this->StartNewCurvePlayback(); // Start a new curve playback for recoil

		}
	}
}

void APlayableCharacter::AttachWeaponsToSockets()
{
	UE_LOG(LogTemp, Log, TEXT("Apa AttachWeaponsToSockets: Attaching weapons to sockets..."));
	for(AWeapon* ChildWeapon : this->Inventory->SpawnedWeapons)
	{
		UE_LOG(LogTemp, Log, TEXT("Apa AttachWeaponsToSockets: Attaching weapon: %s"), *ChildWeapon->GetName());
		if (!IsValid(ChildWeapon->ItemObject)) {
			UE_LOG(LogTemp, Error, TEXT("Apa AttachWeaponsToSockets: Invalid ItemObject. Cannot initialize animations."));
			return; // Invalid item object
		}
		UWeaponObject* WeaponObject = ChildWeapon->GetWeaponObject();
		if (!IsValid(WeaponObject))
		{
			UE_LOG(LogTemp, Error, TEXT("Apa AttachWeaponsToSockets: WeaponObject is null for weapon: %s. Cannot add to inventory."), *ChildWeapon->GetName());
			return; // Invalid weapon object
		}
		EWeaponType WeaponType = WeaponObject->GetWeaponType(); // Get the socket name from the weapon item
		/* If there are no matching socket, the wepaon will be attached to the back */
		FName SocketName = FName(TEXT("BackSocket"));
		/* Get a specific socket if needed (holster...) */
		if (ACustomCharacter::WeaponSocketMap.Contains(WeaponType))
		{
			SocketName = ACustomCharacter::WeaponSocketMap[WeaponType]; // Get the socket name from the map
			UE_LOG(LogTemp, Log, TEXT("Apa AttachWeaponsToSockets: Using socket: %s for weapon type: %s"), *SocketName.ToString(), *UEnum::GetValueAsString(WeaponType));
			//ChildWeapon->WeaponMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;
		}
		/* The current child weapon is the equiped weapon so we need to spawn it in the hand */
		/* Check for OOB first */
		if(this->Inventory->CurrentlyEquipedIndex < this->Inventory->InventoryWeapons.Num() && WeaponObject == this->Inventory->InventoryWeapons[this->Inventory->CurrentlyEquipedIndex])
		{
			SocketName = FName(TEXT("RightHandSocket")); // Attach the current weapon to the right hand socket
			UE_LOG(LogTemp, Log, TEXT("Apa AttachWeaponsToSockets: Override Equiped weapon: %s to socket: %s"), *ChildWeapon->GetName(), *SocketName.ToString());
			//ChildWeapon->WeaponMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson; // Set the weapon mesh to first person type
		}
		
		APlayableCharacter::AttachWeaponToSocket(ChildWeapon, SocketName);
	}
}


void APlayableCharacter::AttachWeaponToSocket(AWeapon* Weapon, FName SocketName)
{
	UE_LOG(LogTemp, Log, TEXT(" Playable character Attaching weapon to socket: %s"), *SocketName.ToString());
	if (Weapon && Weapon->GetWeaponMesh())
	{
		/* First Remove the weapon */
		const FDetachmentTransformRules DetachRules = FDetachmentTransformRules::KeepWorldTransform;
		Weapon->GetWeaponMesh()->DetachFromComponent(DetachRules);

		if (SocketName == FName(TEXT("RightHandSocket")))
		{
			// Attach the weapon to the first person mesh's right hand socket
			if (Weapon->GetWeaponMesh()->AttachToComponent(this->FirstPersonMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName))
			{
				UE_LOG(LogTemp, Log, TEXT("Weapon attached to RightHandSocket successfully."));
			}
		}
		else
		{
			// Attach the weapon mesh to the specified socket on the character's mesh
			if (Weapon->GetWeaponMesh()->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName))
			{
				UE_LOG(LogTemp, Log, TEXT("Weapon attached to socket %s successfully."), *SocketName.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to attach weapon to socket %s."), *SocketName.ToString());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AttachWeaponToSocket: Invalid weapon or weapon mesh."));
	}
}


/**********************
* Save functions      *
***********************/

void APlayableCharacter::SaveWeapons()
{
	UE_LOG(LogTemp, Log, TEXT("Saving weapons..."));
	UPlayerSave* SaveGameInstance = Cast<UPlayerSave>(UGameplayStatics::CreateSaveGameObject(UPlayerSave::StaticClass()));
	uint8 SavedWeaponsCount = 0;
	for(UWeaponObject* WeaponObject : this->Inventory->InventoryWeapons)
	{
		if (WeaponObject)
		{
			FWeaponSaveData WeaponSaveData;
			FString WeaponId = WeaponObject->WeaponId;
			if (WeaponId.IsEmpty())
			{
				UE_LOG(LogTemp, Error, TEXT("Weapon ID is empty for weapon: %s"), *WeaponObject->GetName());
				continue; // Skip saving this weapon if the ID is empty
			}
            
            for (const TPair<FString, UAccessoryObject*>& Pair : WeaponObject->MainAccessories)
            {
				FAccessorySaveData AccessorySaveData;
				AccessorySaveData.AccessoryId = Pair.Value ? Pair.Value->AccessoryId : FString();
				WeaponSaveData.MainAccessories.Add(AccessorySaveData);
            }
			for (const TPair<FString, UAccessoryObject*>& Pair : WeaponObject->OptionnalAccessories)
			{
				FAccessorySaveData AccessorySaveData;
				AccessorySaveData.AccessoryId = Pair.Value ? Pair.Value->AccessoryId : FString();
				WeaponSaveData.OptionnalAccessories.Add(AccessorySaveData);
			}
			if (WeaponObject->Magazine)
			{
				FAccessorySaveData MagazineSaveData;
				MagazineSaveData.AccessoryId = WeaponObject->Magazine->AccessoryId;
				WeaponSaveData.Magazine = MagazineSaveData; // Save the magazine accessory
			}
			SaveGameInstance->PlayerWeapons.Add(WeaponSaveData);
			SavedWeaponsCount++;
		}
	}
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("PlayerSave"), 0);
	UE_LOG(LogTemp, Log, TEXT("Saved %d weapons to PlayerSave."), SavedWeaponsCount);
}

void APlayableCharacter::LoadWeapons()
{
	UE_LOG(LogTemp, Log, TEXT("Loading weapons..."));
	uint8 LoadedWeaponsCount = 0;
	UPlayerSave* SaveGameInstance = Cast<UPlayerSave>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSave"), 0));
	if (SaveGameInstance)
	{
		for (const FWeaponSaveData& WeaponData : SaveGameInstance->PlayerWeapons)
		{
			UWeaponObject* WeaponObject = NewObject<UWeaponObject>();
			if (WeaponData.WeaponId != TEXT("NULL"))
			{
				uint8 RowId = static_cast<uint8>(FCString::Atoi(*WeaponData.WeaponId)); // Convert WeaponId (FString) to uint8
				this->Inventory->CreateWeaponObject(this, RowId); // Create a new weapon object using the data table row ID
				LoadedWeaponsCount++;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load PlayerSave!"));
	}
	UE_LOG(LogTemp, Log, TEXT("Loaded %d weapons from PlayerSave."), LoadedWeaponsCount);
}

void APlayableCharacter::StartNewCurvePlayback()
{
	// Start a new curve playback
	if (IsValid(this->CCurrentWeaponHandRotationCurve) && IsValid(this->CCurrentWeaponHandLocationCurve))
	{
		this->RecoilCurves.Add(FCurvePlayBack(this->CCurrentWeaponHandRotationCurve, this->CCurrentWeaponHandLocationCurve));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Recoil curve is not set! Cannot start playback."));
	}
}

void APlayableCharacter::LoopOverCurvePlaybacks(float DeltaTime)
{
	/* Reset Transform values before adding */
	this->CurrentWeaponTransform = FTransform::Identity; // Reset the transform to identity before applying curves
	for (int32 i = this->RecoilCurves.Num() - 1; i >= 0; --i)
	{
		FCurvePlayBack& Curve = this->RecoilCurves[i];
		Curve.CurrentTime += DeltaTime*Curve.PlayRate;
		if (Curve.GetCurveIsOver())
		{
			this->RecoilCurves.RemoveAt(i);
			continue;
		}
		FVector NewLocation = Curve.CHandLocationCurve->GetVectorValue(Curve.CurrentTime);
		CurrentWeaponTransform = MathUtils::AddVectorToTransformLocation(this->CurrentWeaponTransform, NewLocation);
		FVector NewRotation = Curve.CHandRotationCurve->GetVectorValue(Curve.CurrentTime);
		CurrentWeaponTransform = MathUtils::AddVectorToTransformRotation(this->CurrentWeaponTransform, NewRotation);
	}
	//UE_LOG(LogTemp, Log, TEXT("LoopOverCurvePlaybacks: Looped over %d curves "), this->RecoilCurves.Num());
	/*UE_LOG(LogTemp, Log, TEXT("Final Weapon Transform: Location: %s, Rotation: %s"),
		*CurrentWeaponTransform.GetLocation().ToString(),
		*CurrentWeaponTransform.GetRotation().Rotator().ToString());
	*/
}