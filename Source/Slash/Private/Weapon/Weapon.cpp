#include <Weapon/Weapon.h>
#include <Kismet/GameplayStatics.h>
#include <Components/SphereComponent.h>
#include <Components/BoxComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Interfaces/IHitable.h>

AWeapon::AWeapon() {
	// Initialize box collision component and setup collision presets
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Component"));
	BoxCollisionComponent->SetupAttachment(GetRootComponent());
	BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxCollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// Initialize start/end box trace positions
	BoxTraceStartPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start Position"));
	BoxTraceStartPosition->SetupAttachment(GetRootComponent());
	BoxTraceEndPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End Position"));
	BoxTraceEndPosition->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();

	// Bind event overlap to new function
	BoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::Equip(USceneComponent* parent, FName socketName) {
	// Create transform rules
	FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, false);

	// Attach mesh in parent in the socket transform with transform rules
	mesh->AttachToComponent(parent, transformRules, socketName);

	// Set item state to equipped
	ItemState = EITemState::Equipped;

	// Play sound when equipped
	if(!InteractSound) return;

	UGameplayStatics::PlaySoundAtLocation(this, InteractSound, GetActorLocation());

	// Disable sphere collision after equipping
	if(!SphereCollision) {
		return;
	}

	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::OnShpereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
                                   UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {
	Super::OnShpereStartOverlap(overlappedComponent, otherActor, otherComp, otherBodyIndex, bFromSweep, sweepResult);
}

void AWeapon::OnShpereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex) {
	Super::OnShpereEndOverlap(overlappedComponent, otherActor, otherComp, otherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp,
	int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {

	const FVector start = BoxTraceStartPosition->GetComponentLocation();
	const FVector end = BoxTraceEndPosition->GetComponentLocation();

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(this);

	for (AActor* actor : IgnoreHitActors) {
		actorsToIgnore.AddUnique(actor);
	}
	
	FHitResult hitResult;
	
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		start,
		end,
		FVector(10.f, 10.f, 10.f),
		BoxTraceStartPosition->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		actorsToIgnore,
		EDrawDebugTrace::ForDuration,
		hitResult,
		true
	);

	if(!hitResult.GetActor()) {
		return;
	}

	auto* hitActor = Cast<IHitable>(hitResult.GetActor());

	if(!hitActor) {
		return;
	}

	hitActor->Hit(hitResult.ImpactPoint);
	IgnoreHitActors.AddUnique(hitResult.GetActor());
}
