// Fill out your copyright notice in the Description page of Project Settings.


#include "Items.h"

AItems::AItems() {
	PrimaryActorTick.bCanEverTick = true;
}

void AItems::BeginPlay() {
	Super::BeginPlay();
}

void AItems::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}