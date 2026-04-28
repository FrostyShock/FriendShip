// Fill out your copyright notice in the Description page of Project Settings.

#include "ShovelActor.h"
#include "ID_Registry.h"



// Sets default values
AShovelActor::AShovelActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Physics Mesh"));
	SetRootComponent(RootMesh);
	
}

// Called when the game starts or when spawned
void AShovelActor::BeginPlay()
{
	Super::BeginPlay();
	
	ActorID = FGuid::NewGuid();
	UID_Registry::RegisterActor(this);
}

// Called every frame
void AShovelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


FGuid AShovelActor::GetActorID_Implementation() const
{
	return ActorID;
}

// Implementation of SetActorID (updates the ActorID)
void AShovelActor::SetActorID_Implementation(const FGuid& NewActorID)
{
	ActorID = NewActorID;
}

// Implementation of GetMesh (provides the root static mesh component)
UStaticMeshComponent* AShovelActor::GetMesh_Implementation() const
{
	return RootMesh;
}
