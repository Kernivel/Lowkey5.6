


#include "AbilitySystem/CustomAbilitySystemComponent.h"


UCustomAbilitySystemComponent::UCustomAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}



void UCustomAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}
