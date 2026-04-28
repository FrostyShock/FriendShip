// Fill out your copyright notice in the Description page of Project Settings.


#include "Holdable.h"

#include "ID_Registry.h"

// Sets default values
AHoldable::AHoldable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Physics Mesh"));
	SetRootComponent(RootMesh);
	
	//Preinit the BuoyancyData For First Initialization
	
	BuoyancyComponent = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("BuoyancyComponent"));
	BuoyancyComponent->BuoyancyData = FBuoyancyData();
	SavedBuoyancyData = BuoyancyComponent->BuoyancyData;
	
	ConstructBuoyancy();
}


// Called when the game starts or when spawned
void AHoldable::BeginPlay()
{
	Super::BeginPlay();
	SetPontoonLocations();
	
	ActorID = FGuid::NewGuid();
	UID_Registry::RegisterActor(this);
}

// Called every frame
void AHoldable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AHoldable::SetPontoonLocations()
{
	FVector HullBoundsMax = FVector(0, 0, 0);
	FVector HullBoundsMin = FVector(0, 0, 0);
	RootMesh->GetLocalBounds(HullBoundsMin, HullBoundsMax);
	
	if (PontoonCount == 1)
	{
		BuoyancyComponent->BuoyancyData.Pontoons[0].Radius = PontoonRadius;
		BuoyancyComponent->BuoyancyData.Pontoons[0].RelativeLocation = FVector(0, 0, HullBoundsMax.Z/1.5);
	}
	else
	{
		for (int i = 0; i < PontoonCount; i++)
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
	
	
}


void AHoldable::ConstructBuoyancy()
{
	if (BuoyancyComponent == nullptr)
	{
		return;
	}

	BuoyancyComponent->BuoyancyData = SavedBuoyancyData;
	BuoyancyComponent->BuoyancyData.Pontoons.Empty();

	for (int i = 0; i < PontoonCount; i++)
	{
		BuoyancyComponent->BuoyancyData.Pontoons.Add(FSphericalPontoon());
	}

	SetPontoonLocations();
}


void AHoldable::ToggleBuoyancy()
{
	if (BuoyancyComponent->BuoyancyData.BuoyancyCoefficient != 0 && BuoyancyComponent != nullptr)
	{
		SavedBuoyancyData = BuoyancyComponent->BuoyancyData;
		BuoyancyComponent->BuoyancyData.BuoyancyCoefficient = 0;
	}
	else
	{
		if (BuoyancyComponent != nullptr)
		{
			BuoyancyComponent->BuoyancyData = SavedBuoyancyData;
		}
	}
}


FGuid AHoldable::GetActorID_Implementation() const
{
	return ActorID;
}

// Implementation of SetActorID (updates the ActorID)
void AHoldable::SetActorID_Implementation(const FGuid& NewActorID)
{
	ActorID = NewActorID;
}

// Implementation of GetMesh (provides the root static mesh component)
UStaticMeshComponent* AHoldable::GetMesh_Implementation() const
{
	return RootMesh;
}

void AHoldable::UpdateBuoyantCoefficients(float newBuoyancy)
{
	if (BuoyancyComponent != nullptr)
	{
		BuoyancyComponent->BuoyancyData.BuoyancyCoefficient = newBuoyancy;
	}
}
