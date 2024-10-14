#pragma once

#include "raylib.h"
#include <unordered_set>
#include <string>

#define S_CHUNK_SIZE 16     // Size of each chunk in units
#define S_VIEW_DISTANCE 12   // Number of chunks to load around the player

class SimpleTerrain {
public: 
    SimpleTerrain();
    ~SimpleTerrain();

    void Update(const Vector3& playerPosition);
    void Draw();
    void Unload();

    void SetHeightMeshMap(Vector3 heightMap);

private:
    Vector3 heightMeshMap;

    // Keep track of loaded chunk keys
    std::unordered_set<std::string> loadedChunks;

    // Helper methods
    void LoadChunks(const Vector3& playerPosition);
    void UnloadFarChunks(const Vector3& playerPosition);
    void GenerateChunk(int chunkX, int chunkZ);
    std::string GetChunkKey(int chunkX, int chunkZ);
};
