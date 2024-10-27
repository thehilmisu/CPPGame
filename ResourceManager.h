#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H


#include "raylib.h"
#include <string>
#include <unordered_map>

class ResourceManager {
public:
    // Load all resources
    static void LoadResources();

    // Unload all resources
    static void UnloadResources();

    // Getters for models
    static Model& GetPlayerModel();
    static Model& GetEnemyModel();
    static Model& GetTreeModel();
    static Model& GetCloudModel();
    static Model& GetMissileModel();
    static Model& GetBulletModel();

    // Methods to manage terrain textures
    static void LoadTerrainTexture();
    static void UnloadTerrainTexture();
    static Texture2D& GetTerrainTexture();

    // Methods to manage models
    static void AddTerrainModel(const std::string& key, const Model& model);
    static Model& GetTerrainModel(const std::string& key);
    static void RemoveTerrainModel(const std::string& key);

    static void UnloadAllTerrainResources();

private:
    // Private constructor to prevent instantiation
    ResourceManager() = default;

    // Models
    static Model playerModel;
    static Model enemyModel;
    static Model treeModel;
    static Model cloudModel;
    static Model missileModel;
    static Model bulletModel;

    // Helper function to load a model
    static Model LoadModelFromFile(const std::string& filePath);

    // Terrain resources
    static Texture2D terrainTexture;
    static std::unordered_map<std::string, Model> terrainModels;
};


#endif //RESOURCEMANAGER_H
