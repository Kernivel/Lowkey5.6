


#include "Player/CustomPlayerState.h"
#include "AbilitySystem/CustomAbilitySystemComponent.h"
#include "AbilitySystem/CharacterAttributeSet.h"

ACustomPlayerState::ACustomPlayerState()
{
	SetNetUpdateFrequency(100.f);
	AbilitySystemComponent = CreateDefaultSubobject<UCustomAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ACustomPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCharacterAttributeSet* ACustomPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
