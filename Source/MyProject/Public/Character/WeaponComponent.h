

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "WeaponComponent.generated.h"


UCLASS(Blueprintable)
class MYPROJECT_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	UCameraComponent* PlayerCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool WeaponIsEquiped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	bool bIsFiring = false;

	UFUNCTION(BlueprintCallable, Category = "Fire")
	virtual bool Fire();
protected:
	UFUNCTION()
	virtual void ResetFireState();
	UPROPERTY()
	FTimerHandle FireTimerHandle; // Timer handle for firing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	float DelayBetweenShots = 0.2f; // Delay between shots in secondsa
};
 