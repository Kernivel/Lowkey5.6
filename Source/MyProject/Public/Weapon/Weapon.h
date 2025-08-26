// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon/WeaponObject.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Utils/LightWeaponFireCameraShake.h"
// Load the assets at runtime
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFireEvent);

/**
 * 
 */
UCLASS(Blueprintable)
class MYPROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon(const FObjectInitializer& ObjectInitializer);
	void PostInitializeComponents() override; // Override to initialize components after construction
	UFUNCTION(BlueprintCallable, Category = "Initialization")
	bool Initialize(UWeaponObject* InWeaponObject, bool FirstPersonView);
	UFUNCTION(BlueprintCallable, Category = "Initialization")
	void InitializeWeaponObject();
	UFUNCTION(BlueprintCallable, Category = "Initialization")
	bool InitializeAnimationData();
	UFUNCTION(BlueprintCallable, Category = "Initialization")
	bool InitializeAccessories();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	UFUNCTION(BlueprintCallable, Category = "Fire")
	bool Fire();
	UFUNCTION(BlueprintCallable, Category = "Getter")
	UWeaponObject* GetWeaponObject();
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWeaponFireEvent OnFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	bool bIsFiring = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle")
	UNiagaraSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle")
	FVector MuzzleLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle")
	FRotator MuzzleRotation;
	void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data table")
	TSoftObjectPtr<UDataTable> WeaponDataTable;
	UFUNCTION(BlueprintCallable, Category = "Muzzle")
	FTransform GetMuzzleWorldTransform() const;
private:
	bool InitMesh(bool FirstPersonView);
	FTimerHandle FireTimerHandle; // Timer handle for firing
	void ResetFireState();
};
