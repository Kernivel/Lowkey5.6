

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "CustomPlayerState.generated.h"

class UCustomAbilitySystemComponent;
class UCharacterAttributeSet;
/**
 * 
 */
UCLASS()
class MYPROJECT_API ACustomPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ACustomPlayerState();
	/* IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UCharacterAttributeSet* GetAttributeSet() const;

protected:
	UPROPERTY()
	TObjectPtr<UCustomAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UCharacterAttributeSet> AttributeSet;
};
