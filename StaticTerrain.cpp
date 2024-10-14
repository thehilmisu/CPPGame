#include "StaticTerrain.h"
#include <iostream>

// Constructor
StaticTerrain::StaticTerrain()
    : terrainMesh({ 0 }), terrainModel({ 0 }), terrainTexture({ 0 }),
      terrainWidth(0), terrainLength(0), terrainMaxHeight(0.0f)
{
    LoadResources();
}

// Destructor
StaticTerrain::~StaticTerrain() {
    Unload();
}

// Load resources (textures, etc.)
void StaticTerrain::LoadResources() {
    terrainTexture = LoadTexture("assets/rocky_terrain_diff_4k.png");
    if (terrainTexture.id == 0) {
        std::cerr << "Error: Failed to load terrain texture." << std::endl;
    }
}

// Unload resources
void StaticTerrain::UnloadResources() {
    if (terrainTexture.id != 0) {
        UnloadTexture(terrainTexture);
        terrainTexture.id = 0;
    }
}

// Generate the terrain mesh
void StaticTerrain::GenerateTerrain(int width, int length, float maxHeight) {
    terrainWidth = width;
    terrainLength = length;
    terrainMaxHeight = maxHeight;

    // Generate heightmap image using Perlin noise
    Image heightMap = GenImagePerlinNoise(width, length, 0, 0, 8.0f);

    // Adjust the heightmap to scale the heights
    Color* pixels = LoadImageColors(heightMap);
    for (int i = 0; i < width * length; i++) {
        float normalizedHeight = pixels[i].r / 255.0f; // Assuming grayscale image
        pixels[i].r = pixels[i].g = pixels[i].b = static_cast<unsigned char>(normalizedHeight * 255.0f);
        pixels[i].a = 255;
    }
    UnloadImageColors(pixels);

    // Create the mesh from the heightmap
    Vector3 meshSize = { static_cast<float>(width), maxHeight, static_cast<float>(length) };
    terrainMesh = GenMeshHeightmap(heightMap, meshSize);

    // Upload mesh data to GPU
    UploadMesh(&terrainMesh, false);

    // Create the model from the mesh
    terrainModel = LoadModelFromMesh(terrainMesh);
    terrainModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = terrainTexture;

    // Clean up the image
    UnloadImage(heightMap);
}

// Draw the terrain
void StaticTerrain::Draw() {
    DrawModel(terrainModel, { 0.0f, 0.0f, -50.0f }, 10.0f, WHITE);
}

// Unload the terrain and resources
void StaticTerrain::Unload() {
    UnloadResources();

    if (terrainModel.meshCount > 0) {
        UnloadModel(terrainModel);
        terrainModel.meshCount = 0;
    }

    if (terrainMesh.vertexCount > 0) {
        UnloadMesh(terrainMesh);
        terrainMesh.vertexCount = 0;
    }
}
