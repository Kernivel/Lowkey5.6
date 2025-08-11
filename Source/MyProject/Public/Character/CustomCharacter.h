// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/************************
*Custom files includes  *
*************************/
//Weapons
#include "Weapon/WeaponObject.h" // Include the header for ItemObject
#include "Weapon/Weapon.h"
#include "Weapon/WeaponType.h" // Include the header for WeaponType enum

// Inventory
#include "Inventory/InventoryComponent.h"

/*************************
* Unreal Engine includes *
**************************/
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GroomComponent.h" // Include GroomComponent for hair meshes
#include "Kismet/GameplayStatics.h" // For Damage calculations
#include "CharacterTrajectoryComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "CustomCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireEvent);

UCLASS(Blueprintable)
class MYPROJECT_API ACustomCharacter : public ACharacter
{
	GENERATED_BODY()

	/* Constructor & Construct helpers functions */
public:
	// Sets default values for this character's properties
	ACustomCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PopulateWeaponSocketMap(); // Populate the WeaponSocketMap with weapon types and corresponding socket names
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnFireEvent OnFire;


	/*************************
	* Unreal Functions       *
	**************************/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void ConstructDefaultMetahuman();


	/*************************
	* Custom Public Function *
	**************************/
public:	
	// Redefine the Tick in children classes of ACustomCharacter
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool SwitchToWeapon(int WeaponIndex); // Override the SwitchToWeapon function to use the Inventory component
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void InstanciateAllWeaponItems();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void InstanciateWeaponItem(UWeaponObject* Weapon); // Spawn weapon in the world

	/* Functions to shoot */
	UFUNCTION(BlueprintCallable, Category = "Fire")
	virtual void Fire(); // Function to fire the weapon

	/* Movement Functions */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void BeginSprint(); // Function to begin sprinting
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void EndSprint(); // Function to begin sprinting

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void BeginWalk(); // Function to begin sprinting

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void EndWalk(); // Function to begin sprinting

	/* Valid checks functions */
	UFUNCTION(BlueprintCallable, Category = "Validation")
	virtual bool IsCurrentWeaponValid() const; // Check if the current weapon is valid
	/*************************
	* Public Variables       *
	**************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UInventoryComponent* Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 200.f; // Walk speed of the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunSpeed = 350.f; // Run speed of the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 800.f; // Sprint speed of the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchSpeed = 150.f; // Sprint speed of the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsRunning = false; // Boolean to check if the character is aiming
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
	bool bIsAiming = false; // Boolean to check if the character is aiming
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsCrouching = false; // Boolean to check if the character is crouching
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	bool bIsReloading = false; // Boolean to check if the character is reloading
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trajectory")
	UCharacterTrajectoryComponent* TrajectoryComponent;

	/**********************
	* Reload functions    *
	***********************/
	UFUNCTION(BlueprintCallable, Category = "Reload")
	virtual bool CanReload() const; // Check if the character can reload the current weapon
	UFUNCTION(BlueprintCallable, Category = "Reload")
	virtual uint8 Reload(); // Reload the current weapon if possible
	/*************************
	* Protected Variables    *
	**************************/
protected:
	/* Metahuman meshes*/
	/* Body is already covered in Character.h and accessible through GetMesh() */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MetaHuman")
	USkeletalMeshComponent* TorsoMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MetaHuman")
	USkeletalMeshComponent* PantsMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MetaHuman")
	USkeletalMeshComponent* ShoesMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MetaHuman")
	USkeletalMeshComponent* GlovesMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MetaHuman")
	USkeletalMeshComponent* FaceMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MetaHuman")
	UGroomComponent* HairMesh;

	/* Sockets Map */
	TMap<EWeaponType, FName> WeaponSocketMap; // Map to store weapon type and socket name

	/* Rotation variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BlueprintProtected), Category = "Rotation")
	FRotator PreviousRotation = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BlueprintProtected), Category = "Rotation")
	FRotator CurrentRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BlueprintProtected), Category = "Niagara")
	UNiagaraSystem* ConcreteImpactEffect; // Niagara effect for concrete impact
	/* Protected functions*/
protected:
	/**********************
	* Socket functions    *
	***********************/
	UFUNCTION(BlueprintCallable, Category = "Sockets")
	virtual void AttachWeaponsToSockets();
	UFUNCTION(BlueprintCallable, Category = "Sockets")
	virtual void AttachWeaponToSocket(AWeapon* Weapon, FName SocketName);

	/**********************
	* Roation functions   *
	***********************/
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = "Movement")
	virtual void UpdateRotatorVariables();
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = "Movement")
	virtual FRotator GetRotationDelta(FRotator PrevRotation, FRotator CurrRotation);
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = "Movement")
	virtual FRotator GetRotationSpeedFromRotationDelta(FRotator RotationDelta, float DeltaTime);
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = "Movement")
	virtual FRotator GetRotationSpeed();

};
