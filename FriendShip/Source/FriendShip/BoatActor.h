// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuoyancyComponent.h"
#include "BPI_IDSubsystem.h"
#include "BoatActor.generated.h"

UCLASS()
class FRIENDSHIP_API ABoatActor : public AActor, public IBPI_IDSubsystem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoatActor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ID")
	FGuid ActorID;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBuoyancyComponent* BuoyancyComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* HullCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pontoon")
	float PontoonCount = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pontoon")
	float PontoonRadius = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pontoon")
	float PontoonHeight = 120.0f;
	
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ID")
	FGuid GetActorID() const;
	virtual FGuid GetActorID_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ID")
	void SetActorID(const FGuid& NewActorID);
	virtual void SetActorID_Implementation(const FGuid& NewActorID);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ID")
	UStaticMeshComponent* GetMesh() const;
	virtual UStaticMeshComponent* GetMesh_Implementation() const;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetPontoonLocations();
	
	UFUNCTION(BlueprintCallable)
	void UpdatePontoonHeight(float increaseAmount);
};
