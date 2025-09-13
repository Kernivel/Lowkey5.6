#pragma once
#include "Engine/DataTable.h"
#include "UObject/SoftObjectPtr.h"
#include "Animation/AnimInstance.h"
#include "WeaponAnimationStruct.generated.h"

USTRUCT(BlueprintType)
struct FWeaponAnimation : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FString Name = TEXT("Unknown name");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSoftClassPtr <UAnimInstance> AnimBPClass;

	FWeaponAnimation()
	{
		Name = TEXT("Default Animation");
		AnimBPClass = nullptr; // Default to nullptr if no animation is set
	}
};