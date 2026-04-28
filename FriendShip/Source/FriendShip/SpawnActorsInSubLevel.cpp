// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnActorsInSubLevel.h"          // Header that declares the utility library
#include "Engine/World.h"                  // UWorld, world contexts
#include "Engine/Level.h"                  // ULevel, level structures
#include "Engine/LevelStreaming.h"         // ULevelStreaming, for streaming level queries
#include "Kismet/GameplayStatics.h"

/**
 * --------------------------------------------------------------------
 *  NEW: Return the name of the streamed sub‑level that the supplied
 *  actor currently belongs to.  If the actor is in a level that is
 *  not streamed (e.g., the persistent level) the function still
 *  returns that level’s name.  The return value is a string; callers
 *  can inspect it to decide what to do next.
 * --------------------------------------------------------------------
 */
FString USpawnActorsInSubLevel::GetCurrentSublevelNameFromActor(AActor* Actor, bool& bOutSuccess, FString& OutInfoMessage)
{
    if (!Actor)
    {
        bOutSuccess = false;
        OutInfoMessage = TEXT("Actor is null.");
        return TEXT("");
    }

    UWorld* World = Actor->GetWorld();
    if (!World)
    {
        bOutSuccess = false;
        OutInfoMessage = TEXT("Failed to obtain world from actor.");
        return TEXT("");
    }

    // The level the actor is currently in.
    ULevel* ActorLevel = Actor->GetLevel();
    if (!ActorLevel)
    {
        bOutSuccess = false;
        OutInfoMessage = TEXT("Actor is not associated with a level.");
        return TEXT("");
    }

    // Try to match the level against the world’s streaming levels.
    for (ULevelStreaming* Streaming : World->GetStreamingLevels())
    {
        if (Streaming && Streaming->GetLoadedLevel() == ActorLevel)
        {
            bOutSuccess = true;
            OutInfoMessage = TEXT("Found streamed sub‑level for actor.");
            // Return the package name of the streaming asset – this ends
            // with the level’s name (e.g., "/Game/Levels/MySubLevel").
            FString FullPath = Streaming->GetWorldAssetPackageName();

            // Strip the directory prefix to return only the level name.
            int32 LastSlashIndex = -1;
            if (FullPath.FindLastChar('/', LastSlashIndex))
            {
				FString CleanName = FullPath.RightChop(LastSlashIndex + 1); // "UEPIE_0_map1" or "map1"

				if (CleanName.StartsWith(TEXT("UEDPIE_")))
				{
					CleanName = CleanName.RightChop(FString(TEXT("UEDPIE_")).Len()); // "0_map1" – you can drop the numeric part if you want
					// or simply strip everything up to the first underscore
					int32 Underscore = CleanName.Find(TEXT("_"));
					if (Underscore != INDEX_NONE)
					{
						CleanName = CleanName.RightChop(Underscore + 1); // "map1"
					}
				}
				return CleanName;
            }
            // Fallback if no slash is found.
            return FullPath;
        }
    }

    // Fallback: the actor lives in a non‑streamed level (persistent,
    // level of the game).  Return the level’s simple name.
    bOutSuccess = true;
    OutInfoMessage = TEXT("Actor is in a non‑streamed level.");
    return ActorLevel->GetName();
}

/**
 * ------------------------------------------------------------------
 * 1. Find a sub‑level by name within a given world.
 * ------------------------------------------------------------------
 * This function is intentionally independent of the editor; it only
 * relies on runtime world data.  It first checks streaming levels
 * (which are the usual way sub‑levels are loaded in a packaged game)
 * and then falls back to the normal level array for safety.
 */
ULevel* USpawnActorsInSubLevel::GetSublevelFromWorld(UWorld* World, const FString& SublevelName, bool& bOutSuccess, FString& OutInfoMessage)
{
    if (!World)
    {
        bOutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("World is null. Level Name: %s"), *SublevelName);
        return nullptr;
    }

    // 1️⃣ Search streaming levels first
    for (ULevelStreaming* Streaming : World->GetStreamingLevels())
    {
        if (Streaming && Streaming->GetWorldAssetPackageName().EndsWith(SublevelName))
        {
            if (Streaming->GetLoadedLevel() &&
                Streaming->GetLoadedLevel()->GetName() == SublevelName)
            {
                bOutSuccess = true;
                OutInfoMessage = TEXT("Sublevel found via streaming.");
                return Streaming->GetLoadedLevel();
            }
        }
    }

    // 2️⃣ Fallback: iterate all levels and compare level names directly
    for (ULevel* Level : World->GetLevels())
    {
        if (Level && Level->GetName() == SublevelName)
        {
            bOutSuccess = true;
            OutInfoMessage = TEXT("Sublevel found in world.");
            return Level;
        }
    }

    bOutSuccess = false;
    OutInfoMessage = FString::Printf(TEXT("Sublevel %s not found"), *SublevelName);
    return nullptr;
}


/**
 * ------------------------------------------------------------------
 * 2. Spawn a new actor into a sub‑level at runtime.
 * ------------------------------------------------------------------
 * The function grabs the first valid game world from the global engine
 * context list.  It then obtains the target level via the helper above
 * and uses `SpawnActor` with `OverrideLevel` so the actor is placed
 * in that level’s Actors array.
 */
/*AActor* USpawnActorsInSubLevel::SpawnActorInSublevel(TSubclassOf<AActor> ActorClass, const FString& SublevelName, bool& bOutSuccess, FString& OutInfoMessage)
{
    // Validate the actor class; cannot spawn a null class.
    if (!ActorClass)
    {
        bOutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("ActorClass is null. Level Name: %s"), *SublevelName);
        return nullptr;
    }

    // Grab the first available runtime world (the “game” world).
    UWorld* World = nullptr;
    for (const FWorldContext& Context : GEngine->GetWorldContexts())
    {
        if (Context.World() && Context.World()->IsGameWorld())
        {
            World = Context.World();
            break;
        }
    }
    if (!World)
    {
        bOutSuccess = false;
        OutInfoMessage = TEXT("Failed to obtain runtime world.");
        return nullptr;
    }

    // Find the target level.  If it fails, return early.
    ULevel* TargetLevel = GetSublevelFromWorld(World, SublevelName, bOutSuccess, OutInfoMessage);
    if (!TargetLevel)
        return nullptr;

    // Prepare spawn parameters: override the level and choose a safe collision handling mode.
    FActorSpawnParameters SpawnParams;
    SpawnParams.OverrideLevel = TargetLevel;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // Perform the spawn at world origin with no rotation.
    AActor* SpawnedActor = World->SpawnActor<AActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

    bOutSuccess = true;
    OutInfoMessage = FString::Printf(TEXT("Spawned %s in %s."), *ActorClass->GetName(), *SublevelName);
    return SpawnedActor;
}*/

/**
 * ------------------------------------------------------------------
 * 3. Move or copy a set of existing actors into another sub‑level.
 * ------------------------------------------------------------------
 * The function accepts an array of actors, the destination level name,
 * and a flag indicating whether to copy (true) or move (false).
 * It performs validation, fetches the target level, then processes each actor:
 *   • Copy: spawn a new actor with the same class/transform.
 *   • Move: detach and add the actor to the new level’s Actors list.
 */
/*void USpawnActorsInSubLevel::MoveOrCopyActorsToSublevel(const TArray<AActor*>& Actors, const FString& SublevelName, bool bCopyActor, bool& bOutSuccess, FString& OutInfoMessage)
{
    // Remove any null pointers from the input array.
    TArray<AActor*> ValidActors = Actors;
    ValidActors.RemoveAll([](AActor* A){ return A == nullptr; });

    // Ensure there is something to process.
    if (ValidActors.Num() == 0)
    {
        bOutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("No Actors to Move/Copy. Level Name: %s"), *SublevelName);
        return;
    }

    // Use the world of the first actor (all should be from the same world).
    UWorld* World = ValidActors[0]->GetWorld();
    ULevel* TargetLevel = GetSublevelFromWorld(World, SublevelName, bOutSuccess, OutInfoMessage);
    if (!TargetLevel)
        return;

    if (bCopyActor)
    {
        // Copy each actor by spawning a new instance.
        for (AActor* SrcActor : ValidActors)
        {
            if (!SrcActor) continue;

            FActorSpawnParameters SpawnParams;
            SpawnParams.OverrideLevel = TargetLevel;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            // Duplicate class and transform; additional component copying can be added here.
            AActor* Copy = World->SpawnActor<AActor>(SrcActor->GetClass(),
                                                     SrcActor->GetActorTransform(), SpawnParams);
            // (Optional) Copy component data if needed.
        }
    }
    else // Move
    {
        for (AActor* Actor : ValidActors)
        {
            if (!Actor) continue;

            // Detach from its current parent so it can be reassigned.
            Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

            // Insert into the new level’s Actors array.
            TargetLevel->Actors.Add(Actor);

            // Small visibility toggle to force UE to update level association.
            Actor->SetActorHiddenInGame(true);
            Actor->SetActorHiddenInGame(false);
        }
    }

    bOutSuccess = true;
    OutInfoMessage = FString::Printf(TEXT("%s %s to %s."), bCopyActor ? TEXT("Copied") : TEXT("Moved"), ValidActors.Num() > 1 ? TEXT("actors") : TEXT("actor"), *SublevelName);
}*/

/**
 * ------------------------------------------------------------------
 * 4. Runtime helper to change an actor’s owning level.
 * ------------------------------------------------------------------
 * This convenience function is a thin wrapper around the logic in
 * MoveOrCopyActorsToSublevel.  It operates on a single actor,
 * detaches it, and moves it to the requested level.
 */
/*bool USpawnActorsInSubLevel::ChangeActorLevel(AActor* Actor, const FString& TargetLevelName, FString& OutMsg)
{
    if (!Actor)
    {
        OutMsg = TEXT("Actor is null.");
        return false;
    }

    UWorld* World = Actor->GetWorld();
    if (!World)
    {
        OutMsg = TEXT("Failed to get world from actor.");
        return false;
    }

    bool bFound = false;
    ULevel* TargetLevel = GetSublevelFromWorld(World, TargetLevelName, bFound, OutMsg);
    if (!TargetLevel) return false;

    // Detach the actor from its previous level
    Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    // **Register the actor with the new level** – this updates its internal Level pointer
    //TargetLevel->AddActor(Actor);          // internally calls Actor->SetLevel(TargetLevel)

    // Optionally toggle visibility to force UE to acknowledge the move
    Actor->SetActorHiddenInGame(true);
    Actor->SetActorHiddenInGame(false);

    OutMsg = FString::Printf(TEXT("Actor moved to %s."), *TargetLevelName);
    return true;
    
}*/

UWorld* USpawnActorsInSubLevel::GetWorldFromActor(AActor* Actor, bool& bOutSuccess, FString& OutInfoMessage)
{
    if (!Actor)
    {
        bOutSuccess = false;
        OutInfoMessage = TEXT("Actor is null.");
        return nullptr;
    }

    bOutSuccess = true;
    OutInfoMessage = TEXT("Actor world retrieved.");
    return Actor->GetWorld();
}