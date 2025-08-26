

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CustomAbilitySystemComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UCustomAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UCustomAbilitySystemComponent();
protected:
	virtual void BeginPlay() override;
};
