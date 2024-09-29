#include "SimpleTerrain.h"
#include "GameUtilities.h"
#include <cmath>


SimpleTerrain::SimpleTerrain() : heightMeshMap((Vector3){16.0f, 0.1f, 16.0f})
{
    //heightMap = LoadImage("assets/heightmap.png"); // Load your height map image
    terrainMesh = GenMeshHeightmap(heightMap, heightMeshMap); 
    terrainModel = LoadModelFromMesh(terrainMesh);
    
    terrainTexture = LoadTexture("assets/coast_sand_05_diff_4k.png"); // Optional texture for the terrain
    terrainModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = terrainTexture;
}

SimpleTerrain::~SimpleTerrain()
{

}

void SimpleTerrain::Update(Vector3 playerPosition)
{
    static Vector3 lastPlayerPosition = { 0.0f, 0.0f, 0.0f };
    if (fabs(playerPosition.x - lastPlayerPosition.x) > S_CHUNK_SIZE || 
        fabs(playerPosition.z - lastPlayerPosition.z) > S_CHUNK_SIZE) {

        LoadChunks(playerPosition);
        lastPlayerPosition = playerPosition;
    }
}


TerrainChunk SimpleTerrain::GenerateChunk(Vector3 chunkPosition) 
{
    TerrainChunk chunk;

    int resolution = 32;
    heightMap = GenImagePerlinNoise(resolution, resolution,
                                         chunkPosition.x * S_CHUNK_SIZE, chunkPosition.z * S_CHUNK_SIZE,
                                         500.0f);  // Adjust scale for detail

    //heightMeshMap.y = 10.0f * GameUtilities::PerlinNoise(chunkPosition.x, chunkPosition.z);  // Scale the height value

    chunk.mesh = GenMeshHeightmap(heightMap, heightMeshMap);
    UploadMesh(&chunk.mesh, false);


    chunk.model = LoadModelFromMesh(chunk.mesh);
    chunk.position = chunkPosition;
    chunk.texture = terrainTexture;
    chunk.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = chunk.texture;
    chunk.active = true;

    return chunk;
}

void SimpleTerrain::LoadChunks(Vector3 playerPosition) 
{
    terrainChunks.clear();
    int playerChunkX = (int)floor(playerPosition.x / S_CHUNK_SIZE);
    int playerChunkZ = (int)floor(playerPosition.z / S_CHUNK_SIZE);

    //load chunks
    for (int z = -S_VIEW_DISTANCE; z <= S_VIEW_DISTANCE; z++) {
        for (int x = -S_VIEW_DISTANCE; x <= S_VIEW_DISTANCE; x++) {
            Vector3 chunkPos = {(float)(playerChunkX + x) * S_CHUNK_SIZE, 0.0f, (float)(playerChunkZ + z) * S_CHUNK_SIZE};
            TerrainChunk chunk = GenerateChunk(chunkPos);
            terrainChunks.push_back(chunk);
        }
    }

    for (int i = terrainChunks.size() - 1; i >= 0; i--)
    {
        int distanceX = std::abs(terrainChunks.at(i).position.x / S_CHUNK_SIZE - playerChunkX);
        int distanceZ = std::abs(terrainChunks.at(i).position.z / S_CHUNK_SIZE - playerChunkZ);

        if (distanceX > S_VIEW_DISTANCE || distanceZ > S_VIEW_DISTANCE)
        {
            UnloadModel(terrainChunks.at(i).model);
            UnloadTexture(terrainChunks.at(i).texture);
            
            // Erase the chunk from the vector
            terrainChunks.erase(terrainChunks.begin() + i);
        }
    }


}

void SimpleTerrain::Draw()
{
    for (auto& chunk : terrainChunks) {
        DrawModel(chunk.model, chunk.position, 1.1f, WHITE);
    }
}

void SimpleTerrain::SetHeightmap(Vector3 hMap)
{
    heightMeshMap = hMap;
}

void SimpleTerrain::Unload()
{
    //UnloadModel(terrainModel);

    for(auto& chunk : terrainChunks){
        UnloadModel(chunk.model);
        UnloadTexture(chunk.texture);
    }
        
    UnloadTexture(terrainTexture);
    UnloadImage(heightMap);
    UnloadMesh(terrainMesh);
} 