#include <Breakable/Breakable.h>
#include <GeometryCollection/GeometryCollectionComponent.h>
#include <Items/Collectable.h>

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
	if(CollectableItemsList.IsEmpty() || m_isBroken) return;

	m_isBroken = true;
	
	GeometryCollection->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	auto spawnLocation = GetActorLocation();
	spawnLocation.Z += 75.f; 

	auto randomItemIndex = FMath::RandRange(0, CollectableItemsList.Num() - 1);
	
	GetWorld()->SpawnActor<ACollectable>(CollectableItemsList[randomItemIndex], spawnLocation, GetActorRotation());
}

