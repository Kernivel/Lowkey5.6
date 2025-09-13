

#pragma once

#include "CoreMinimal.h"
#include "ActorItem.generated.h"


UCLASS(Blueprintable)
class MYPROJECT_API AActorItem : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AActorItem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
