// Fill out your copyright notice in the Description page of Project Settings.


#include "BoatActor.h"

#include "ID_Registry.h"

// Sets default values
ABoatActor::ABoatActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BuoyancyComponent = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("BuoyancyComponent"));
	HullCollision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HullCollision"));
	SetRootComponent(HullCollision);
	
	BuoyancyComponent->BuoyancyData = FBuoyancyData();
	if (BuoyancyComponent != nullptr)
	{
		
		for (int i = 0; i < 4; i++)
		{
			BuoyancyComponent->BuoyancyData.Pontoons.Add(FSphericalPontoon());
		}
	}
	
	
	
}

void ABoatActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	
}
	


// Called when the game starts or when spawned
void ABoatActor::BeginPlay()
{
	Super::BeginPlay();
	SetPontoonLocations();
	
	ActorID = FGuid::NewGuid();
	UID_Registry::RegisterActor(this);
}

// Called every frame
void ABoatActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoatActor::SetPontoonLocations()
{
	FVector HullBoundsMax = FVector(0, 0, 0);
	FVector HullBoundsMin = FVector(0, 0, 0);
	HullCollision->GetLocalBounds(HullBoundsMin, HullBoundsMax);
	
	for (int i = 0; i < 4; i++)
	{
		BuoyancyComponent->BuoyancyData.Pontoons[i].Radius = PontoonRadius;
		switch (i)
		{
		case 0:
			BuoyancyComponent->BuoyancyData.Pontoons[i].RelativeLocation = FVector(HullBoundsMax.X, HullBoundsMax.Y, PontoonHeight);
			break;
		case 1:
			BuoyancyComponent->BuoyancyData.Pontoons[i].RelativeLocation = FVector(-HullBoundsMax.X, -HullBoundsMax.Y, PontoonHeight);
			break;
		case 2:
			BuoyancyComponent->BuoyancyData.Pontoons[i].RelativeLocation = FVector(-HullBoundsMax.X, HullBoundsMax.Y, PontoonHeight);
			break;
		case 3:
			BuoyancyComponent->BuoyancyData.Pontoons[i].RelativeLocation = FVector(HullBoundsMax.X, -HullBoundsMax.Y, PontoonHeight);
			break;
		default:
			BuoyancyComponent->BuoyancyData.Pontoons[i].RelativeLocation = FVector(0, 0, 0);
			break;
		}
		
	}
	
}

void ABoatActor::UpdatePontoonHeight(float increaseAmount)
{
	float newPontoonHeight = BuoyancyComponent->BuoyancyData.Pontoons[0].RelativeLocation.Z + increaseAmount;
	
	for (int i = 0; i < 4; i++)
	{
		BuoyancyComponent->BuoyancyData.Pontoons[i].RelativeLocation = FVector(BuoyancyComponent->BuoyancyData.Pontoons[i].RelativeLocation.X, BuoyancyComponent->BuoyancyData.Pontoons[i].RelativeLocation.Y, newPontoonHeight);
	}
}


FGuid ABoatActor::GetActorID_Implementation() const
{
	return ActorID;
}

// Implementation of SetActorID (updates the ActorID)
void ABoatActor::SetActorID_Implementation(const FGuid& NewActorID)
{
	ActorID = NewActorID;
}

// Implementation of GetMesh (provides the root static mesh component)
UStaticMeshComponent* ABoatActor::GetMesh_Implementation() const
{
	return HullCollision;
}