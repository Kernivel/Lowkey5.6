// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/************************
*Custom files includes  *
*************************/
//Weapons
#include "Item/Weapon/WeaponObject.h" // Include the header for ItemObject
#include "Item/Weapon/Weapon.h"
#include "Item/Weapon/WeaponType.h" // Include the header for WeaponType enum
// GAS
#include "AbilitySystemInterface.h"

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerFireEvent);

class UCustomAbilitySystemComponent;
class UCharacterAttributeSet;
class UGameplayAbility;
class UGameplayEffect;

UCLASS(Blueprintable)
class MYPROJECT_API ACustomCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/***************************************************
	* CONSTRUCTOR & CONSTRUCT HELPERS FUNCTIONS        *
	***************************************************/
public:
	// Sets default values for this character's properties
	ACustomCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PopulateWeaponSocketMap(); // Populate the WeaponSocketMap with weapon types and corresponding socket names
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerFireEvent OnFire;

	/*************************
	* UNREAL FUNCTIONS       *
	**************************/
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnConstruction(const FTransform& Transform) override;	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	void ConstructDefaultMetahuman();


	/*************************
	* CUSTOM PUBLIC FUNCTION *
	**************************/
public:	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool SwitchToWeapon(int WeaponIndex); // Override the SwitchToWeapon function to use the Inventory component
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void InstanciateAllWeaponItems();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void InstanciateWeaponItem(UWeaponObject* Weapon); // Spawn weapon in the world
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool IsCurrentWeaponValid() const; // Check if the current weapon is valid

	/**********************
	* Shoot functions  *
	***********************/
	UFUNCTION(BlueprintCallable, Category = "Fire")
	virtual void Fire(); // Function to fire the weapon

	/**********************
	* Movement functions  *
	***********************/
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void BeginSprint(); // Function to begin sprinting
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void EndSprint(); // Function to begin sprinting

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void BeginWalk(); // Function to begin sprinting

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void EndWalk(); // Function to begin sprinting


	/**********************
	* Reload functions    *
	***********************/
	UFUNCTION(BlueprintCallable, Category = "Reload")
	virtual bool CanReload() const; // Check if the character can reload the current weapon
	UFUNCTION(BlueprintCallable, Category = "Reload")
	virtual uint8 Reload(); // Reload the current weapon if possible

	/*************************
	* PUBLIC VARIABLES       *
	**************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UInventoryComponent* Inventory;
	/**********************
	* Movement variables  *
	***********************/
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsCrouching = false; // Boolean to check if the character is crouching

	/**********************
	* Actions variables    *
	***********************/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bHAsWeapon = false; // Boolean to check if the has a weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
	bool bIsAiming = false; // Boolean to check if the character is aiming
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	bool bIsReloading = false; // Boolean to check if the character is reloading
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trajectory")
	UCharacterTrajectoryComponent* TrajectoryComponent;


	/*************************
	* PROTECTED VARIABLES    *
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
	USkeletalMeshComponent* FaceMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MetaHuman")
	UGroomComponent* HairMesh;

	/******************
	* GAS Variables    *
	*******************/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCustomAbilitySystemComponent> AbilitySystemComponent;	
	UPROPERTY()
	TObjectPtr< UCharacterAttributeSet> AttributeSet;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;


	TMap<EWeaponType, FName> WeaponSocketMap; // Map to store weapon type and socket name

	/* Rotation variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BlueprintProtected), Category = "Rotation")
	FRotator PreviousRotation = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BlueprintProtected), Category = "Rotation")
	FRotator CurrentRotation = FRotator::ZeroRotator;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BlueprintProtected), Category = "Niagara")
	UNiagaraSystem* ConcreteImpactEffect; // Niagara effect for concrete impact


	/*************************
	* PROTECTED FUNCTIONS    *
	**************************/
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

	/**********************
	* GAS Functions       *
	**********************/
	void GiveDefaultAbilities();

	void InitDefaultAttributes();
};
