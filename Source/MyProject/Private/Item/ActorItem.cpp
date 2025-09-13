


#include "Item/ActorItem.h"


// Sets default values for this component's properties
AActorItem::AActorItem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts
void AActorItem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void AActorItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




