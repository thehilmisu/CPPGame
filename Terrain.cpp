// Terrain.cpp

#include "Terrain.h"
#include "raymath.h"  // Include for vector math functions
#include <cmath>

Terrain::Terrain()
{
    // Constructor
}

Terrain::~Terrain()
{
}

void Terrain::Initialize()
{
    // Load the terrain texture
    terrainTexture = LoadTexture("assets/rocky_terrain_diff_4k.png");
}

void Terrain::Update(Vector3 playerPosition)
{
    int playerChunkX = (int)std::floor(playerPosition.x / CHUNK_SIZE);
    int playerChunkZ = (int)std::floor(playerPosition.z / CHUNK_SIZE);

    // Load nearby chunks
    for (int z = playerChunkZ - VIEW_DISTANCE; z <= playerChunkZ + VIEW_DISTANCE; z++)
    {
        for (int x = playerChunkX - VIEW_DISTANCE; x <= playerChunkX + VIEW_DISTANCE; x++)
        {
            auto chunkKey = GetChunkKey(x, z);
            if (terrainChunks.find(chunkKey) == terrainChunks.end())
            {
                // Load new chunk
                TerrainChunk chunk;
                chunk.index = { (float)x, (float)z };
                chunk.model = GenerateTerrainChunkModel(x, z);
                chunk.isLoaded = true;
                terrainChunks[chunkKey] = chunk;
            }
        }
    }

    // Unload distant chunks
    for (auto it = terrainChunks.begin(); it != terrainChunks.end();)
    {
        int chunkX = it->first.first;
        int chunkZ = it->first.second;
        int distanceX = abs(chunkX - playerChunkX);
        int distanceZ = abs(chunkZ - playerChunkZ);

        if (distanceX > VIEW_DISTANCE || distanceZ > VIEW_DISTANCE)
        {
            // Unload chunk
            UnloadModel(it->second.model);
            it = terrainChunks.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Terrain::Draw()
{
    for (const auto& pair : terrainChunks)
    {
        const TerrainChunk& chunk = pair.second;
        Vector3 chunkPosition = {
            chunk.index.x * CHUNK_SIZE,
            0.0f,  // No additional Y-offset needed
            chunk.index.y * CHUNK_SIZE
        };
        DrawModel(chunk.model, chunkPosition, 1.0f, WHITE);
    }
}

void Terrain::Unload()
{
    // Unload all terrain chunks
    for (auto& pair : terrainChunks)
    {
        UnloadModel(pair.second.model);
    }
    terrainChunks.clear();

    // Unload terrain texture
    UnloadTexture(terrainTexture);
}

Model Terrain::GenerateTerrainChunkModel(int chunkX, int chunkZ)
{
    // Generate heightmap image
    int resolution = CHUNK_RESOLUTION;
    Image image = GenImagePerlinNoise(resolution, resolution,
                                      chunkX * CHUNK_SIZE, chunkZ * CHUNK_SIZE,
                                      20.0f);  // Adjust scale for detail

    // Generate mesh from heightmap
    Mesh mesh = GenMeshHeightmap(image, (Vector3){ CHUNK_SIZE, HEIGHT_SCALE, CHUNK_SIZE });
    UnloadImage(image);  // Unload image after generating mesh

    // Shift the mesh down by HEIGHT_SCALE to start terrain from Y = 0
    for (int i = 0; i < mesh.vertexCount; i++)
    {
        mesh.vertices[i * 3 + 1] -= HEIGHT_SCALE;
    }

    // Upload mesh data to GPU
    UploadMesh(&mesh, false);

    // Load model from mesh
    Model model = LoadModelFromMesh(mesh);

    // Assign terrain texture
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = terrainTexture;

    return model;
}

unsigned int Terrain::GetChunkSeed(int chunkX, int chunkZ)
{
    // Generate a unique seed for each chunk
    return (unsigned int)(chunkX * 73856093 ^ chunkZ * 19349663);
}

std::pair<int, int> Terrain::GetChunkKey(int chunkX, int chunkZ)
{
    return std::make_pair(chunkX, chunkZ);
}

float Terrain::GetHeightAtPosition(Vector3 position)
{
    int chunkX = (int)std::floor(position.x / CHUNK_SIZE);
    int chunkZ = (int)std::floor(position.z / CHUNK_SIZE);
    auto chunkKey = GetChunkKey(chunkX, chunkZ);

    if (terrainChunks.find(chunkKey) != terrainChunks.end())
    {
        TerrainChunk& chunk = terrainChunks[chunkKey];
        Vector3 localPosition = {
            position.x - chunkX * CHUNK_SIZE,
            0.0f,
            position.z - chunkZ * CHUNK_SIZE
        };

        Mesh mesh = chunk.model.meshes[0];
        // Assuming the mesh is a grid with known resolution
        float gridSpacing = CHUNK_SIZE / (CHUNK_RESOLUTION - 1);

        int xIndex = (int)(localPosition.x / gridSpacing);
        int zIndex = (int)(localPosition.z / gridSpacing);

        // Clamp indices
        xIndex = Clamp(xIndex, 0, CHUNK_RESOLUTION - 2);
        zIndex = Clamp(zIndex, 0, CHUNK_RESOLUTION - 2);

        // Get vertex indices
        int vertexIndex = (zIndex * CHUNK_RESOLUTION + xIndex) * 3;

        // Access the Y components of the four surrounding vertices
        float h1 = mesh.vertices[vertexIndex + 1];                       // Top-left
        float h2 = mesh.vertices[vertexIndex + 1 + 3];                   // Top-right
        float h3 = mesh.vertices[vertexIndex + 1 + CHUNK_RESOLUTION * 3]; // Bottom-left
        float h4 = mesh.vertices[vertexIndex + 1 + CHUNK_RESOLUTION * 3 + 3]; // Bottom-right

        // Calculate fractional positions
        float fracX = (localPosition.x - xIndex * gridSpacing) / gridSpacing;
        float fracZ = (localPosition.z - zIndex * gridSpacing) / gridSpacing;

        // Bilinear interpolation
        float heightTop = Lerp(h1, h2, fracX);
        float heightBottom = Lerp(h3, h4, fracX);
        float height = Lerp(heightTop, heightBottom, fracZ);

        return height;
        //return 0.0f;
    }
    else
    {
        // Chunk not loaded; return default height
        return 0.0f;
    }
}

