// SimpleTerrain.h

#ifndef SIMPLETERRAIN_H
#define SIMPLETERRAIN_H

#include "raylib.h"


class SimpleTerrain {

public: 
    SimpleTerrain();
    ~SimpleTerrain();

    void SetModel(Model model);
    Model GetModel();

    void SetHeightmap(Vector3 hMap);

    void Update(float deltaTime, Vector3 playerPosition);
    void Unload();

private:
    Model terrainModel;
    Texture2D terrainTexture;
    Image heightMap;
    Mesh terrainMesh;
    Vector3 heightMeshMap;

};

#endif