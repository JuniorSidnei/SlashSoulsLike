#include <Weapon/Weapon.h>
#include <Kismet/GameplayStatics.h>
#include <Components/SphereComponent.h>
#include <Components/BoxComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Interfaces/IHitable.h>
#include <NiagaraComponent.h>

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

void AWeapon::Equip(USceneComponent* parent, FName socketName, AActor* newOwner, APawn* newInstigator) {

	// Set new owner and instigator of the weapon
	SetOwner(newOwner);
	SetInstigator(newInstigator);
	
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

	if(!sparkEffect) {
		return;
	}

	sparkEffect->Deactivate();
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

	// Set start/end position of box collision to get the impact point
	const FVector startPos = BoxTraceStartPosition->GetComponentLocation();
	const FVector endPos = BoxTraceEndPosition->GetComponentLocation();

	// Actors to ignore
	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(this);

	for (AActor* actor : IgnoreHitActors) {
		actorsToIgnore.AddUnique(actor);
	}
	
	FHitResult hitResult;

	// Box trace to detect point
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		startPos,
		endPos,
		FVector(5.f, 5.f, 5.f),
		BoxTraceStartPosition->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		actorsToIgnore,
		EDrawDebugTrace::ForDuration,
		hitResult,
		true
	);

	// We didn't hit, just return
	if(!hitResult.GetActor()) {
		return;
	}

	// create fields in impact position
	CreateFields(hitResult.ImpactPoint);
	
	// The hit actor
	auto* hitActor = Cast<IHitable>(hitResult.GetActor());

	// Add actor to ignore list after hit
	IgnoreHitActors.AddUnique(hitResult.GetActor());
	
	// If isn't a hittable object, just return
	if(!hitActor) {
		return;
	}

	// Deal damage to actor hit
	UGameplayStatics::ApplyDamage(hitResult.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());

	// Call actor hit function
	hitActor->Execute_Hit(hitResult.GetActor(), hitResult.ImpactPoint);
}
