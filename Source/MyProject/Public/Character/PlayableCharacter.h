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

/* Recoil shakes */
#include "Utils/LightWeaponFireCameraShake.h"

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

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MetaHuman|Camera")
	UCameraComponent* CameraComponent;

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
	AItem* ScanForPickableItems();

protected:
	class UInputMappingContext* InputMapping = NULL;

	class UInputAction* Shoot = NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MetaHuman")
	USkeletalMeshComponent* FirstPersonMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MetaHuman")
	USkeletalMeshComponent* FirstPersonGlovesMesh;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetInputSystem();

	virtual void SetupPlayerInputCallBacks();

	/* Curves variables for Recoil */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	UCurveVector* CCurrentWeaponCameraRotationCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	UCurveVector* CCurrentWeaponCameraLocationCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	UCurveVector* CCurrentWeaponHandRotationCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	UCurveVector* CCurrentWeaponHandLocationCurve;
	/* DONT USE POINTERS FOR STRUCT IN TARRAYS */


	/* Recoils Transform variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FTransform CurrentCameraTransform = FTransform::Identity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FTransform CurrentWeaponTransform = FTransform::Identity;

	/* Storing the current looked at Item */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scanning")
	AItem* CurrentLookedAtItem = nullptr;

	virtual void AttachWeaponToSocket(AWeapon* Weapon, FName SocketName) override;

	virtual void AttachWeaponsToSockets() override;

};
