// SimpleTerrain.h

#ifndef SIMPLETERRAIN_H
#define SIMPLETERRAIN_H

#include "raylib.h"
#include <vector>


#define S_CHUNK_SIZE 16     // Size of each chunk in units
#define S_VIEW_DISTANCE 6   // Number of chunks to load around the player


struct TerrainChunk {
    Mesh mesh;
    Model model;
    Texture2D texture;
    Vector3 position;
    bool active;
};

class SimpleTerrain {

public: 
    SimpleTerrain();
    ~SimpleTerrain();

    void SetModel(Model model);
    Model GetModel();

    void SetHeightmap(Vector3 hMap);

    void Update(Vector3 playerPosition);
    void Draw();
    void Unload();

    void LoadChunks(Vector3 playerPosition);

private:
    Model terrainModel;
    Texture2D terrainTexture;
    Image heightMap;
    Mesh terrainMesh;
    Vector3 heightMeshMap;
    std::vector<TerrainChunk> terrainChunks;

    TerrainChunk GenerateChunk(Vector3 chunkPosition);
    


};

#endif