// Terrain.cpp

#include "Terrain.h"
#include "raymath.h"      // Include for vector math functions
#include <cmath>
#include <iostream>

// Define Clamp and Lerp functions
template <typename T>
T ClampValue(T value, T min, T max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

template <typename T>
T LerpValue(T start, T end, float alpha)
{
    return start + alpha * (end - start);
}

Terrain::Terrain()
{
    // Constructor
}

Terrain::~Terrain()
{
    Unload();
}

void Terrain::Initialize()
{
    // Load terrain texture atlas
    terrainAtlas = LoadTexture("assets/grass.png");

    // Check if texture loaded successfully
    if (terrainAtlas.id == 0)
    {
        std::cerr << "Error loading terrain texture atlas!\n";
    }
    else
    {
        std::cout << "Terrain texture atlas loaded successfully.\n";
    }
}

void Terrain::Update(Vector3 playerPosition)
{
    int playerChunkX = static_cast<int>(std::floor(playerPosition.x / CHUNK_SIZE));
    int playerChunkZ = static_cast<int>(std::floor(playerPosition.z / CHUNK_SIZE));

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
                chunk.index = { static_cast<float>(x), static_cast<float>(z) };
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
        int distanceX = std::abs(chunkX - playerChunkX);
        int distanceZ = std::abs(chunkZ - playerChunkZ);

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
            0.0f,  // Terrain starts from Y = 0
            chunk.index.y * CHUNK_SIZE
        };

        // Draw the model with the texture atlas
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

    // Unload terrain texture atlas
    UnloadTexture(terrainAtlas);
}

Model Terrain::GenerateTerrainChunkModel(int chunkX, int chunkZ)
{

    // Generate heightmap image using Perlin noise
    int resolution = CHUNK_RESOLUTION;
    Image heightmap = GenImagePerlinNoise(resolution, resolution,
                                         chunkX * CHUNK_SIZE, chunkZ * CHUNK_SIZE,
                                         200.0f);  // Adjust scale for detail
                                         
    // Generate mesh from heightmap
    Mesh mesh = GenMeshHeightmap(heightmap, (Vector3){ CHUNK_SIZE, HEIGHT_SCALE, CHUNK_SIZE });
    UnloadImage(heightmap);  // Unload image after generating mesh

    // Shift the mesh down by HEIGHT_SCALE to start terrain from Y = 0
    for (int i = 0; i < mesh.vertexCount; i++)
    {
        mesh.vertices[i * 3 + 1] -= HEIGHT_SCALE;
    }

    // Modify UVs based on height to map to texture atlas regions
    // Assume texture atlas has 2x2 grid:
    // (0,0) - Sand
    // (0.5,0) - Grass
    // (0,0.5) - Rock
    // (0.5,0.5) - Snow
    for (int i = 0; i < mesh.vertexCount; i++)
    {
        float y = mesh.vertices[i * 3 + 1];
        float alpha = (y + HEIGHT_SCALE) / HEIGHT_SCALE; // Normalize height between 0 and 1

        // Determine which texture region to use based on height
        // 0.0 - 0.25: Sand
        // 0.25 - 0.5: Grass
        // 0.5 - 0.75: Rock
        // 0.75 - 1.0: Snow
        float u_min, v_min, u_max, v_max;

        if (alpha <= 0.25f)
        {
            // Sand (0,0) to (0.5,0.5)
            u_min = 0.0f;
            v_min = 0.0f;
            u_max = 0.5f;
            v_max = 0.5f;
        }
        else if (alpha <= 0.5f)
        {
            // Grass (0.5,0.0) to (1.0,0.5)
            u_min = 0.5f;
            v_min = 0.0f;
            u_max = 1.0f;
            v_max = 0.5f;
        }
        else if (alpha <= 0.75f)
        {
            // Rock (0,0.5) to (0.5,1.0)
            u_min = 0.0f;
            v_min = 0.5f;
            u_max = 0.5f;
            v_max = 1.0f;
        }
        else
        {
            // Snow (0.5,0.5) to (1.0,1.0)
            u_min = 0.5f;
            v_min = 0.5f;
            u_max = 1.0f;
            v_max = 1.0f;
        }

        // Original UV coordinates
        float u = mesh.texcoords[i * 2 + 0];
        float v = mesh.texcoords[i * 2 + 1];

        // Scale UVs to the selected texture region
        mesh.texcoords[i * 2 + 0] = LerpValue(u_min, u_max, u);
        mesh.texcoords[i * 2 + 1] = LerpValue(v_min, v_max, v);
    }

    // Upload modified mesh data to GPU
    UploadMesh(&mesh, true);

    // Load model from mesh
    Model model = LoadModelFromMesh(mesh);

    // Assign texture atlas to the model's material
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = terrainAtlas;

    return model;
}

unsigned int Terrain::GetChunkSeed(int chunkX, int chunkZ)
{
    // Generate a unique seed for each chunk
    return static_cast<unsigned int>(chunkX * 73856093 ^ chunkZ * 19349663);
}

std::pair<int, int> Terrain::GetChunkKey(int chunkX, int chunkZ)
{
    return std::make_pair(chunkX, chunkZ);
}

float Terrain::GetHeightAtPosition(Vector3 position)
{
    int chunkX = static_cast<int>(std::floor(position.x / CHUNK_SIZE));
    int chunkZ = static_cast<int>(std::floor(position.z / CHUNK_SIZE));
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

        int xIndex = static_cast<int>(localPosition.x / gridSpacing);
        int zIndex = static_cast<int>(localPosition.z / gridSpacing);

        // Clamp indices to valid range
        xIndex = ClampValue(xIndex, 0, CHUNK_RESOLUTION - 2);
        zIndex = ClampValue(zIndex, 0, CHUNK_RESOLUTION - 2);

        // Get vertex indices
        int vertexIndex = (zIndex * CHUNK_RESOLUTION + xIndex) * 3;

        // Access the Y components of the four surrounding vertices
        float h1 = mesh.vertices[vertexIndex + 1];                             // Top-left
        float h2 = mesh.vertices[vertexIndex + 1 + 3];                         // Top-right
        float h3 = mesh.vertices[vertexIndex + 1 + CHUNK_RESOLUTION * 3];      // Bottom-left
        float h4 = mesh.vertices[vertexIndex + 1 + CHUNK_RESOLUTION * 3 + 3];  // Bottom-right

        // Calculate fractional positions
        float fracX = (localPosition.x - xIndex * gridSpacing) / gridSpacing;
        float fracZ = (localPosition.z - zIndex * gridSpacing) / gridSpacing;

        // Bilinear interpolation
        float heightTop = LerpValue(h1, h2, fracX);
        float heightBottom = LerpValue(h3, h4, fracX);
        float height = LerpValue(heightTop, heightBottom, fracZ);

        return height;
    }

    return 0.0f;
}
