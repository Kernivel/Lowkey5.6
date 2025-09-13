#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Item/Weapon/WeaponSaveStruct.h"
#include "PlayerSave.generated.h"

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UPlayerSave : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
	TArray<FWeaponSaveData> PlayerWeapons;

};