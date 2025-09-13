// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Item/ItemObject.h"
#include "Item.generated.h"

UCLASS(Blueprintable)
class MYPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem(const FObjectInitializer& ObjectInitializer);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
protected:
private:
	bool InitCollisionSphere();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsPickedUp = false; // State to check if the item is picked up
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bCanBePickedUp = true; // State to check if the item can be picked up
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UItemObject* ItemObject; // Item object data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	USphereComponent* CollisionSphere;
};
