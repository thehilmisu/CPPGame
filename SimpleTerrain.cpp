#include "SimpleTerrain.h"
#include "ResourceManager.h"
#include <cmath>
#include <vector>

SimpleTerrain::SimpleTerrain() : heightMeshMap({16.0f, 0.1f, 16.0f}) {
    // Load the terrain texture using ResourceManager
    ResourceManager::LoadTerrainTexture();
}

SimpleTerrain::~SimpleTerrain() {
    //Unload();
}

void SimpleTerrain::Update(const Vector3& playerPosition) {
    static Vector3 lastPlayerPosition = { 0.0f, 0.0f, 0.0f };
    if (fabs(playerPosition.x - lastPlayerPosition.x) > S_CHUNK_SIZE ||
        fabs(playerPosition.z - lastPlayerPosition.z) > S_CHUNK_SIZE) {

        LoadChunks(playerPosition);
        UnloadFarChunks(playerPosition);
        lastPlayerPosition = playerPosition;
    }
}

void SimpleTerrain::LoadChunks(const Vector3& playerPosition) {
    int playerChunkX = static_cast<int>(floor(playerPosition.x / S_CHUNK_SIZE));
    int playerChunkZ = static_cast<int>(floor(playerPosition.z / S_CHUNK_SIZE));

    for (int z = -S_VIEW_DISTANCE; z <= S_VIEW_DISTANCE; ++z) {
        for (int x = -S_VIEW_DISTANCE; x <= S_VIEW_DISTANCE; ++x) {
            int chunkX = playerChunkX + x;
            int chunkZ = playerChunkZ + z;
            std::string chunkKey = GetChunkKey(chunkX, chunkZ);

            if (loadedChunks.find(chunkKey) == loadedChunks.end()) {
                GenerateChunk(chunkX, chunkZ);
                loadedChunks.insert(chunkKey);
            }
        }
    }
}

void SimpleTerrain::UnloadFarChunks(const Vector3& playerPosition) {
    int playerChunkX = static_cast<int>(floor(playerPosition.x / S_CHUNK_SIZE));
    int playerChunkZ = static_cast<int>(floor(playerPosition.z / S_CHUNK_SIZE));

    std::vector<std::string> chunksToUnload;

    for (const auto& chunkKey : loadedChunks) {
        int chunkX, chunkZ;
        sscanf(chunkKey.c_str(), "%d_%d", &chunkX, &chunkZ);

        int distanceX = abs(chunkX - playerChunkX);
        int distanceZ = abs(chunkZ - playerChunkZ);

        if (distanceX > S_VIEW_DISTANCE || distanceZ > S_VIEW_DISTANCE) {
            chunksToUnload.push_back(chunkKey);
        }
    }

    for (const auto& chunkKey : chunksToUnload) {
        ResourceManager::RemoveTerrainModel(chunkKey);
        //ResourceManager::RemoveTerrainMesh(chunkKey);
        loadedChunks.erase(chunkKey);
    }
}


void SimpleTerrain::GenerateChunk(int chunkX, int chunkZ) {
    std::string chunkKey = GetChunkKey(chunkX, chunkZ);

    // Generate heightmap image
    int resolution = 32;
    float noiseScale = 0.1f;
    Image heightMap = GenImagePerlinNoise(resolution, resolution,
                                          chunkX * S_CHUNK_SIZE * noiseScale,
                                          chunkZ * S_CHUNK_SIZE * noiseScale,
                                          800.0f);

    // Generate mesh from heightmap
    Mesh mesh = GenMeshHeightmap(heightMap, heightMeshMap);
    UploadMesh(&mesh, false);

    // Create model from mesh
    Model model = LoadModelFromMesh(mesh);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = ResourceManager::GetTerrainTexture();

    // Store model in ResourceManager
    ResourceManager::AddTerrainModel(chunkKey, model);

    // Clean up
    UnloadImage(heightMap);
}


void SimpleTerrain::Draw() {
    for (const auto& chunkKey : loadedChunks) {
        int chunkX, chunkZ;
        sscanf(chunkKey.c_str(), "%d_%d", &chunkX, &chunkZ);
        Vector3 position = { chunkX * S_CHUNK_SIZE, 0.0f, chunkZ * S_CHUNK_SIZE };

        Model& model = ResourceManager::GetTerrainModel(chunkKey);
        DrawModel(model, position, 1.0f, WHITE);
    }
}

void SimpleTerrain::Unload() {
    static bool unloaded = false;
    if (!unloaded) {
        ResourceManager::UnloadAllTerrainResources();
        ResourceManager::UnloadTerrainTexture();
        unloaded = true;
    }
}

std::string SimpleTerrain::GetChunkKey(int chunkX, int chunkZ) {
    return std::to_string(chunkX) + "_" + std::to_string(chunkZ);
}
