// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpawnActorsInSubLevel.generated.h"

class ULevel;
class UWorld;

/**
 * Utility library for spawning and moving actors between levels at runtime.
 */
UCLASS()
class FRIENDSHIP_API USpawnActorsInSubLevel : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Retrieve the UWorld that owns the supplied actor.
	 *  Returns nullptr if Actor is null.  The success flag and message
	 *  are for consistency with the other utilities. */
	UFUNCTION(BlueprintCallable, Category = "Spawn Actors in Sublevel")
	static FString GetCurrentSublevelNameFromActor(AActor* Actor, bool& bOutSuccess, FString& OutInfoMessage);
	
	UFUNCTION(BlueprintCallable, Category = "Spawn Actors in Sublevel")
	static ULevel* GetSublevelFromWorld(UWorld* World, const FString& SublevelName, bool& bOutSuccess, FString& OutInfoMessage);

	/** Spawn a new actor of the specified class into the named sub‑level. */
	//UFUNCTION(BlueprintCallable, Category = "Spawn Actors in Sublevel")
	//static AActor* SpawnActorInSublevel(TSubclassOf<AActor> ActorClass,
	//	const FString& SublevelName, bool& bOutSuccess, FString& OutInfoMessage);

	/** Move or copy existing actors into the target sub‑level. */
	//UFUNCTION(BlueprintCallable, Category = "Spawn Actors in Sublevel")
	//static void MoveOrCopyActorsToSublevel(const TArray<AActor*>& Actors,
	//	const FString& SublevelName, bool bCopyActor, bool& bOutSuccess, FString& OutInfoMessage);

	/** Runtime helper that changes an actor’s owning level.  Returns true on success. */
	//UFUNCTION(BlueprintCallable, Category = "Spawn Actors in Sublevel")
	//static bool ChangeActorLevel(AActor* Actor, const FString& TargetLevelName, FString& OutMsg);

	UFUNCTION(BlueprintCallable, Category = "Spawn Actors in Sublevel")
	static UWorld* GetWorldFromActor(AActor* Actor, bool& bOutSuccess, FString& OutInfoMessage);
};
