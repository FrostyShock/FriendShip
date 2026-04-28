// Fill out your copyright notice in the Description page of Project Settings.


#include "ID_Registry.h"
#include "BPI_IDSubsystem.h"
#include "Components/StaticMeshComponent.h"

TMap<FGuid, TWeakObjectPtr<AActor>> UID_Registry::ActorRegistry;
TMap<FGuid, TWeakObjectPtr<UStaticMeshComponent>> UID_Registry::ComponentRegistry;

UID_Registry::UID_Registry()
{
}

void UID_Registry::RegisterActor(AActor* Actor)
{
	if (!Actor || !Actor->GetClass()->ImplementsInterface(UBPI_IDSubsystem::StaticClass()))
	{
		return;
	}

	const FGuid ActorID = IBPI_IDSubsystem::Execute_GetActorID(Actor);
	if (!ActorID.IsValid())
	{
		return;
	}

	ActorRegistry.Add(ActorID, Actor);

	if (UStaticMeshComponent* MeshComp = IBPI_IDSubsystem::Execute_GetMesh(Actor))
	{
		ComponentRegistry.Add(ActorID, MeshComp);
	}
}

void UID_Registry::UnregisterActor(AActor* Actor)
{
	if (!Actor || !Actor->GetClass()->ImplementsInterface(UBPI_IDSubsystem::StaticClass()))
	{
		return;
	}

	const FGuid ActorID = IBPI_IDSubsystem::Execute_GetActorID(Actor);

	ActorRegistry.Remove(ActorID);
	ComponentRegistry.Remove(ActorID);
}

AActor* UID_Registry::GetActorByID(const FGuid ActorID)
{
	if (TWeakObjectPtr<AActor>* Found = ActorRegistry.Find(ActorID))
	{
		if (Found->IsValid())
		{
			return Found->Get();
		}

		ActorRegistry.Remove(ActorID);
	}
	return nullptr;
}

UStaticMeshComponent* UID_Registry::GetComponentByID(FGuid ActorID)
{
	if (TWeakObjectPtr<UStaticMeshComponent>* Found = ComponentRegistry.Find(ActorID))
	{
		if (Found->IsValid())
		{
			return Found->Get();
		}

		ComponentRegistry.Remove(ActorID);
	}

	return nullptr;
}


FGuid UID_Registry::GetIDByActor(AActor* Actor)
{
	if (!Actor || !Actor->GetClass()->ImplementsInterface(UBPI_IDSubsystem::StaticClass()))
	{
		return FGuid();
	}

	return IBPI_IDSubsystem::Execute_GetActorID(Actor);
}

bool UID_Registry::SwapActorParams(const FGuid& ActorID1, const FGuid& ActorID2)
{
	AActor* Actor1 = GetActorByID(ActorID1);
	AActor* Actor2 = GetActorByID(ActorID2);

	if (!Actor1 || !Actor2)
	{
		return false;
	}

	const FVector PosA = Actor1->GetActorLocation();
	const FRotator RotA = Actor1->GetActorRotation();
	const FVector ScaleA = Actor1->GetActorScale3D();

	const FVector PosB = Actor2->GetActorLocation();
	const FRotator RotB = Actor2->GetActorRotation();
	const FVector ScaleB = Actor2->GetActorScale3D();

	Actor1->SetActorLocationAndRotation(PosB, RotB);
	Actor1->SetActorScale3D(ScaleB);

	Actor2->SetActorLocationAndRotation(PosA, RotA);
	Actor2->SetActorScale3D(ScaleA);

	/*UStaticMeshComponent* MeshA = IInt_Interactions::Execute_GetMesh(Actor1);
	UStaticMeshComponent* MeshB = IInt_Interactions::Execute_GetMesh(Actor2);

	if (MeshA && MeshB)
	{
		UStaticMesh* TempMesh = MeshA->GetStaticMesh();
		MeshA->SetStaticMesh(MeshB->GetStaticMesh());
		MeshB->SetStaticMesh(TempMesh);
	}*/

	return true;
}

bool UID_Registry::SwapActorID(const FGuid& ActorID1, const FGuid& ActorID2)
{
	AActor* Actor1 = GetActorByID(ActorID1);
	AActor* Actor2 = GetActorByID(ActorID2);

	if (!Actor1 || !Actor2)
	{
		return false;
	}

	if (!Actor1->GetClass()->ImplementsInterface(UBPI_IDSubsystem::StaticClass()) ||
		!Actor2->GetClass()->ImplementsInterface(UBPI_IDSubsystem::StaticClass()))
	{
		return false;
	}

	const FGuid ID1 = IBPI_IDSubsystem::Execute_GetActorID(Actor1);
	const FGuid ID2 = IBPI_IDSubsystem::Execute_GetActorID(Actor2);

	ActorRegistry.Remove(ID1);
	ActorRegistry.Remove(ID2);
	ComponentRegistry.Remove(ID1);
	ComponentRegistry.Remove(ID2);

	IBPI_IDSubsystem::Execute_SetActorID(Actor1, ID2);
	IBPI_IDSubsystem::Execute_SetActorID(Actor2, ID1);

	ActorRegistry.Add(ID2, Actor1);
	ActorRegistry.Add(ID1, Actor2);

	if (UStaticMeshComponent* Mesh1 = IBPI_IDSubsystem::Execute_GetMesh(Actor1))
	{
		ComponentRegistry.Add(ID2, Mesh1);
	}
	if (UStaticMeshComponent* Mesh2 = IBPI_IDSubsystem::Execute_GetMesh(Actor2))
	{
		ComponentRegistry.Add(ID1, Mesh2);
	}

	return true;
}

// Destructor implementation
UID_Registry::~UID_Registry()
{
}