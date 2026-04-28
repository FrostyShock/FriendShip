// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnActorsInSubLevel.h"          // Header that declares the utility library
#include "Engine/World.h"                  // UWorld, world contexts
#include "Engine/Level.h"                  // ULevel, level structures
#include "Engine/LevelStreaming.h"         // ULevelStreaming, for streaming level queries
#include "Kismet/GameplayStatics.h"

// Returns the Owning world context for an actor and removes the filepath from it. Useful for determining if an actor
// that crosses a stream threshold should be elligible for reparenting. 

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
                
                // Allows the use of function with the PIE for debugging purposes - filepath outputs with different format.
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


// Returns the plaintext name of any open level if the Persistent Level is not the bottom most level open. 
// Generally deprecated for this project, but has some utility.

ULevel* USpawnActorsInSubLevel::GetSublevelFromWorld(UWorld* World, const FString& SublevelName, bool& bOutSuccess, FString& OutInfoMessage)
{
    if (!World)
    {
        bOutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("World is null. Level Name: %s"), *SublevelName);
        return nullptr;
    }

    // Search streaming levels first
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

    // Fallback: iterate all levels and compare level names directly
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