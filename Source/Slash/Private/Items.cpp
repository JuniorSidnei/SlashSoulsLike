#include <Items.h>
#include <Components/SphereComponent.h>

AItems::AItems() {
	PrimaryActorTick.bCanEverTick = true;

	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = m_mesh;

	m_sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	m_sphereCollision->SetupAttachment(GetRootComponent());
}

void AItems::BeginPlay() {
	Super::BeginPlay();

	m_sphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AItems::OnShpereStartOverlap);
	m_sphereCollision->OnComponentEndOverlap.AddDynamic(this, &AItems::OnShpereEndOverlap);
}

float AItems::TransformedSin() const {
	return Amplitude * FMath::Sin(m_runningTime * TimeConstant);
}

float AItems::TransformedCos() const {
	return Amplitude * FMath::Cos(m_runningTime * TimeConstant);
}

void AItems::OnShpereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {
	
}

void AItems::OnShpereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex) {
	
}

void AItems::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	m_runningTime += DeltaTime;
}
