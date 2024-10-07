// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Items.h>
#include <Weapon.generated.h>

UCLASS()
class SLASH_API AWeapon : public AItems {
	GENERATED_BODY()

public:
	void Equip(USceneComponent* parent, FName socketName);
	
protected:
	virtual void OnShpereStartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) override;
	virtual void OnShpereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex) override;
};
