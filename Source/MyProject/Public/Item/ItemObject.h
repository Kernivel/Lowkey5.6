// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemObject.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MYPROJECT_API UItemObject : public UObject
{
	GENERATED_BODY()
public:
protected:
	UPROPERTY(BlueprintGetter = GetItemObjectName, BlueprintSetter = SetItemObjectName)
	FString Name;
	UPROPERTY(BlueprintGetter = GetItemObjectDescription, BlueprintSetter = SetItemObjectDescription)
	FText Description; // Description of the item
	UPROPERTY(BlueprintGetter = GetItemObjectImage, BlueprintSetter = SetItemObjectImage)
	TSoftObjectPtr<UTexture2D> Image; // Icon of the item, using TSoftObjectPtr for lazy loading
public :
	/* Getters and Setter */
	// BlueprintPure can be used to create a getter, it allows the function to be called in Blueprints without executing any logic, just returning a value.
	
	// Getter and Setter for Name
	UFUNCTION(BlueprintPure)
	FString GetItemObjectName() const { return Name; } // Getter for Name
	UFUNCTION(BlueprintCallable)
	void SetItemObjectName(FString NewName) {Name = NewName;} // Setter for Name

	// Getter and Setter for Description
	UFUNCTION(BlueprintPure)
	FText GetItemObjectDescription() const { return Description; } // Getter for Description
	UFUNCTION(BlueprintCallable)
	void SetItemObjectDescription(FText NewDescription) { Description = NewDescription; } // Setter for Description

	// Getter and Setter for Image
	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UTexture2D> GetItemObjectImage() const { return Image; } // Getter for Image
	UFUNCTION(BlueprintCallable)
	void SetItemObjectImage(TSoftObjectPtr<UTexture2D> NewImage) { Image = NewImage; } // Setter for Image
};
