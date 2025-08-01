#pragma once

#include "RecoilModificationStruct.generated.h"

/* All the values neccesary to reduce or increase the recoil */
USTRUCT(BlueprintType)
struct FRecoilModification
{
	GENERATED_BODY()
	/* Recoil variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Recoil|Up")
	float FlatUpRecoil = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Recoil|Up")
	float MultiUpRecoil = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Recoil|Right")
	float FlatRightRecoil = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Recoil|Right")
	float MultiRightRecoil = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Recoil|Back")
	float FlatBackRecoil = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Recoil|Back")
	float MultiBackRecoil = 1;
	/* Accuracy variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Accuracy|Spread")
	float FlatMinSpread = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Accuracy|Spread")
	float MultMinSpread = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Accuracy|Spread")
	float FlatMaxSpread = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Accuracy|Spread")
	float MultMaxSpread = 1.0f;
	/* How fast the Spread grows with each shot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Accuracy|Spread")
	float FlatSpreadGrowth = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Accuracy|Spread")
	float MultSpreadGrowth = 1.0f;
	/* How fast the Spread shrinks after not shooting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Accuracy|Spread")
	float FlatSpreadShrink = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Accuracy|Spread")
	float MultSpreadShrink = 1.0f;

};