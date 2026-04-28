// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BPI_IDSubsystem.h"
#include "ShovelActor.generated.h"

UCLASS()
class FRIENDSHIP_API AShovelActor : public AActor, public IBPI_IDSubsystem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShovelActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ID")
	FGuid ActorID;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* RootMesh;
	
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
