// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuoyancyComponent.h"
#include "BPI_IDSubsystem.h"
#include "Holdable.generated.h"

UCLASS()
class FRIENDSHIP_API AHoldable : public AActor, public IBPI_IDSubsystem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHoldable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ID")
	FGuid ActorID;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBuoyancyComponent* BuoyancyComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* RootMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pontoon")
	float PontoonCount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pontoon")
	float PontoonRadius = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pontoon")
	float PontoonHeight = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pontoon")
	float NewBuoyancyCoefficient = 0;
	
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
	FBuoyancyData SavedBuoyancyData;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ConstructBuoyancy();
	void SetPontoonLocations();
	
	UFUNCTION(BlueprintCallable)
	void ToggleBuoyancy();
	
	UFUNCTION(BlueprintCallable)
	void UpdateBuoyantCoefficients(float newBuoyancy);
	
};
