#include <Slash/Public/Items/Items.h>
#include <Components/SphereComponent.h>
#include <Characters/BarbarousPlayer.h> 

AItems::AItems() {
	PrimaryActorTick.bCanEverTick = true; 

	// Create mesh component 
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = mesh;

	// Create sphere collsion component
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(GetRootComponent());
}

void AItems::BeginPlay() {
	Super::BeginPlay();

	// Bind the actions begin/end overlap
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AItems::OnShpereStartOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AItems::OnShpereEndOverlap);
}

float AItems::TransformedSin() const {
	return Amplitude * FMath::Sin(m_runningTime * TimeConstant);
}

float AItems::TransformedCos() const {
	return Amplitude * FMath::Cos(m_runningTime * TimeConstant);
}

void AItems::OnShpereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {

	// Cast the other actor to barbarous player
	auto barbarousPlayer = Cast<ABarbarousPlayer>(otherActor);

	// If cast fail, return
	if(!barbarousPlayer) return;

	// Set the current overlapping item 
	barbarousPlayer->SetOverlappingItem(this);
}

void AItems::OnShpereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex) {

	// Cast the other actor to barbarous player
	auto barbarousPlayer = Cast<ABarbarousPlayer>(otherActor);

	// If cast fail, return
	if(!barbarousPlayer) return;

	// Set current item to null
	barbarousPlayer->SetOverlappingItem(nullptr);
}

void AItems::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	m_runningTime += DeltaTime;

	if(ItemState != EITemState::Hovering) return;
	
	AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
}