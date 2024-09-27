// Terrain.h

#ifndef TERRAIN_H
#define TERRAIN_H

#include "raylib.h"
#include <map>
#include <utility>  // For std::pair
#include <vector>

// Define the chunk size and view distance
#define CHUNK_SIZE 256.0f
#define CHUNK_RESOLUTION 128    // Number of vertices per side
#define VIEW_DISTANCE 2         // Number of chunks to load around the player

// Define the height scale
#define HEIGHT_SCALE 2.0f


class Terrain
{
public:
    Terrain();
    ~Terrain();

    // Initialize the terrain system (load textures, etc.)
    void Initialize();

    // Update terrain chunks based on the player's position
    void Update(Vector3 playerPosition);

    // Draw all loaded terrain chunks
    void Draw();

    // Clean up resources
    void Unload();

    // Get terrain height at a given position
    float GetHeightAtPosition(Vector3 position);

private:
    struct TerrainChunk
    {
        Vector2 index;      // Chunk index (e.g., (0,0), (1,0), etc.)
        Model model;        // Terrain model for this chunk
        bool isLoaded;      // Whether the chunk is currently loaded
    };

    // Map to store loaded terrain chunks
    std::map<std::pair<int, int>, TerrainChunk> terrainChunks;

    // Texture for the terrain
    Texture2D terrainTexture;

    // Generate a terrain chunk model at the given index
    Model GenerateTerrainChunkModel(int chunkX, int chunkZ);

    // Helper function to generate a unique seed for each chunk
    unsigned int GetChunkSeed(int chunkX, int chunkZ);

    // Helper function to get the key for the terrainChunks map
    std::pair<int, int> GetChunkKey(int chunkX, int chunkZ);
};

#endif // TERRAIN_H
