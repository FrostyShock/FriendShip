// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ID_Registry.generated.h"
/**
 * 
 */
UCLASS()

class FRIENDSHIP_API UID_Registry : public UObject
{
	GENERATED_BODY()
public:
	UID_Registry();
	
	static void RegisterActor(AActor* Actor);
	static void UnregisterActor(AActor* Actor);
	
	UFUNCTION(BlueprintCallable, Category="Registry")
	static AActor* GetActorByID(FGuid ActorID);
	
	UFUNCTION(BlueprintCallable, Category="Registry")
	static UStaticMeshComponent* GetComponentByID(FGuid ActorID);
	
	UFUNCTION(BlueprintCallable, Category="Registry")
	static FGuid GetIDByActor(AActor* Actor);
	
	UFUNCTION(BlueprintCallable, Category="Registry")
	static bool SwapActorParams(const FGuid& ActorID1, const FGuid& ActorID2);
	
	UFUNCTION(BlueprintCallable, Category="Registry")
	static bool SwapActorID(const FGuid& ActorID1, const FGuid& ActorID2);
	
	~UID_Registry();

private:
	static TMap<FGuid, TWeakObjectPtr<AActor>> ActorRegistry;
	static TMap<FGuid, TWeakObjectPtr<UStaticMeshComponent>> ComponentRegistry;
};
	
