#include <Slash/Public/Items/Items.h>
#include <Components/SphereComponent.h>
#include <Interfaces/Pickable.h>
#include <NiagaraComponent.h>

AItems::AItems() {
	PrimaryActorTick.bCanEverTick = true; 
	
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = mesh;
	mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(GetRootComponent());
	
	sparkEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SparkEffect"));
	sparkEffect->SetupAttachment(GetRootComponent());
}

void AItems::BeginPlay() {
	Super::BeginPlay();

	// Bind the actions begin/end overlap
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AItems::OnSphereStartOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AItems::OnSphereEndOverlap);
}

float AItems::TransformedSin() const {
	return Amplitude * FMath::Sin(m_runningTime * TimeConstant);
}

float AItems::TransformedCos() const {
	return Amplitude * FMath::Cos(m_runningTime * TimeConstant);
}

void AItems::OnSphereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {

	// Cast the other actor to barbarous player
	auto pickable = Cast<IPickable>(otherActor);

	// If cast fail, return
	if(!pickable) return;

	// Set the current overlapping item 
	pickable->SetOverlappingItem(this);
}

void AItems::OnSphereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex) {

	// Cast the other actor to barbarous player
	auto pickable = Cast<IPickable>(otherActor);

	// If cast fail, return
	if(!pickable) return;

	// Set current item to null
	pickable->SetOverlappingItem(nullptr);
}

void AItems::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	m_runningTime += DeltaTime;

	if(ItemState != EITemState::Hovering) return;

	AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
	AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
}