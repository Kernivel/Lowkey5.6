// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomCharacter.h"
#include "Async/ParallelFor.h"


#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "Curves/CurveVector.h"
#include "Camera/CameraComponent.h"

#include "Item/ItemPickup.h"
#include "Character/WeaponComponent.h"
/* Recoil shakes */
#include "Utils/LightWeaponFireCameraShake.h"

/*Debug net*/
#include "Engine/Engine.h"
#include "Engine/NetDriver.h"

/* Saves */
#include "Utils/PlayerSave.h"

#include "Utils/CurvePlayBack.h"
#include "Utils/MathUtils.h"
#include "PlayableCharacter.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MYPROJECT_API APlayableCharacter : public ACustomCharacter
{
	GENERATED_BODY()

private:
	void ConstructorLoadInputActions();
	
public:
	APlayableCharacter();

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void RetryInitAbilitySystem();
	virtual void OnRep_PlayerState() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MetaHuman|Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component|Weapon")
	UWeaponComponent* WeaponComponent;

	virtual void Fire() override;

	/**********************
	* Save functions      *
	***********************/
	UFUNCTION(BlueprintCallable, Category = "Save")
	void SaveWeapons();
	UFUNCTION(BlueprintCallable, Category = "Save")
	void LoadWeapons();
	
	/************************
	* Instancing functions  *
	*************************/
	//UFUNCTION(BlueprintCallable, Category = "Instanciate") Already defined in CustomCharacter.h
	void InstanciateAllWeaponItems();
	//UFUNCTION(BlueprintCallable, Category = "Instanciate") Already defined in CustomCharacter.h
	void InstanciateWeaponItem(UWeaponObject* Weapon);


	/************************
	*Scanning functions     *
	*************************/
	UFUNCTION(BlueprintCallable, Category = "Scanning")
	void ScanForInteractable();

	/*************************
	* Recoil		         *
	*************************/
	/* Curves variables for Recoil */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	TArray<FCurvePlayBack> RecoilCurves; // Array of curves for recoil playback
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	UCurveVector* CCurrentWeaponHandRotationCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	UCurveVector* CCurrentWeaponHandLocationCurve;
	UFUNCTION(BlueprintCallable, Category = "Recoil")
	void StartNewCurvePlayback();
	UFUNCTION(BlueprintCallable, Category = "Recoil")
	void LoopOverCurvePlaybacks(float DeltaTime);

protected:
	class UInputMappingContext* InputMapping = NULL;

	class UInputAction* Shoot = NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MetaHuman")
	USkeletalMeshComponent* FirstPersonMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MetaHuman")
	USkeletalMeshComponent* FirstPersonGlovesMesh;


	virtual void Tick(float DeltaTime) override;

	virtual void GetInputSystem();

	virtual void SetupPlayerInputCallBacks();


	/* DONT USE POINTERS FOR STRUCT IN TARRAYS */


	/* Recoils Transform variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FTransform CurrentWeaponTransform = FTransform::Identity;

	/* Storing the current looked at Item */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scanning")
	AItemPickup* CurrentLookedAtItemPickup = nullptr;

	virtual void AttachWeaponToSocket(AWeapon* Weapon, FName SocketName) override;

	virtual void AttachWeaponsToSockets() override;

private:
	void InitAbilitySystemComponent();
};
