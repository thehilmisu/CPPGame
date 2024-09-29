#include "SimpleTerrain.h"


SimpleTerrain::SimpleTerrain() : heightMeshMap((Vector3){16.0f, 6.0f, 16.0f})
{
    heightMap = LoadImage("assets/heightmap.png"); // Load your height map image
    terrainMesh = GenMeshHeightmap(heightMap, heightMeshMap); 
    // The middle value in Vector3 scales height; set it low for a flatter terrain
    terrainModel = LoadModelFromMesh(terrainMesh);
    
    terrainTexture = LoadTexture("assets/terrain_diffuse.png"); // Optional texture for the terrain
    terrainModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = terrainTexture;
}

SimpleTerrain::~SimpleTerrain()
{

}

void SimpleTerrain::Update(float deltaTime, Vector3 playerPosition)
{

}

void SimpleTerrain::SetHeightmap(Vector3 hMap)
{
    heightMeshMap = hMap;
}

void SimpleTerrain::SetModel(Model model)
{
    terrainModel = model;
}

Model SimpleTerrain::GetModel()
{
    return terrainModel;
}

void SimpleTerrain::Unload()
{
    UnloadModel(terrainModel);
    UnloadTexture(terrainTexture);
    UnloadImage(heightMap);
    UnloadMesh(terrainMesh);
}