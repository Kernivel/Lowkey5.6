#pragma once

#include "BaseRecoilStruct.generated.h"

/* All the base values to modify the base curve of the recoil 
* (might be merge into a single file with RecoilModificationStruct.h if too redundant)
* The weapon first kicks up with a spike, then resets back to the original position following
* a spring-like curve to simulate the human error when going back to the original position.
*/
USTRUCT(BlueprintType)
struct FBaseRecoilValues
{	
	GENERATED_BODY()
	/* Value to determine how hard and how fast the weapon kicks */
	float BaseRecoilSpikeIntensity = 1.0f;
	float BaseRecoilSpikeSpeed = 1.0f;

	/* Value to determine how much the weapon will kick back after the spike */
	float CriticalDamping = 0.5f; // How much the spring resists oscillation 0= > no oscillation (very bouncy), 1= critical damped  (no bounce back)
	float Siffness = 1.0f; // How strong the spring pulls towards the target. Higher values = faster, more oscillation.

};