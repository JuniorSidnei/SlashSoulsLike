#include <Breakable/Breakable.h>
#include <GeometryCollection/GeometryCollectionComponent.h>

ABreakable::ABreakable() {
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}


void ABreakable::BeginPlay() {
	Super::BeginPlay();
	
}

void ABreakable::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ABreakable::Hit_Implementation(const FVector& impactPoint) {
	
}

