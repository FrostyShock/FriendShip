// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BPI_IDSubsystem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBPI_IDSubsystem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FRIENDSHIP_API IBPI_IDSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ID")
	FGuid GetActorID() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ID")
	void SetActorID(const FGuid& NewActorID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ID")
	UStaticMeshComponent* GetMesh() const;
public:
};
