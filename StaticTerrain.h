#pragma once

#include "raylib.h"

class StaticTerrain {
public:
    StaticTerrain();
    ~StaticTerrain();

    void GenerateTerrain(int width, int length, float maxHeight);
    void Draw();
    void Unload();

private:
    Mesh terrainMesh;
    Model terrainModel;
    Texture2D terrainTexture;
    int terrainWidth;
    int terrainLength;
    float terrainMaxHeight;

    void LoadResources();
    void UnloadResources();
};
