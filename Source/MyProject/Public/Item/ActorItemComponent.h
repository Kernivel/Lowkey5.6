

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorItemComponent.generated.h"


UCLASS(Blueprintable)
class MYPROJECT_API UActorItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
