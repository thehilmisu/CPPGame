#include "ResourceManager.h"
#include "GameSettings.h"
#include <iostream>

// Initialize static members
Model ResourceManager::playerModel = { 0 };
Model ResourceManager::enemyModel = { 0 };
Model ResourceManager::treeModel = { 0 };
Model ResourceManager::cloudModel = { 0 };
Model ResourceManager::missileModel = { 0 };
Model ResourceManager::bulletModel = { 0 };

Texture2D ResourceManager::terrainTexture = { 0 };
std::unordered_map<std::string, Model> ResourceManager::terrainModels;

void ResourceManager::LoadResources() {
    // Load models
    playerModel = LoadModelFromFile(PLAYER_RAFALE_OBJ);
    enemyModel = LoadModelFromFile("assets/Enemy/enemy.obj");
    treeModel = LoadModelFromFile("assets/Tree/Tree.obj");
    cloudModel = LoadModelFromFile("assets/Clouds/Cloud_Polygon_Blender_1.obj");
    missileModel = LoadModelFromFile("assets/Missile/missile.obj");
    bulletModel = LoadModelFromFile("assets/Bullet/bullet.obj");
}


Model& ResourceManager::GetPlayerModel() {
    return playerModel;
}

Model& ResourceManager::GetEnemyModel() {
    return enemyModel;
}

Model& ResourceManager::GetTreeModel() {
    return treeModel;
}

Model& ResourceManager::GetCloudModel() {
    return cloudModel;
}

Model& ResourceManager::GetMissileModel() {
    return missileModel;
}

Model& ResourceManager::GetBulletModel() {
    return bulletModel;
}

Model ResourceManager::LoadModelFromFile(const std::string& filePath) {
    Model model = LoadModel(filePath.c_str());
    if (model.meshCount == 0) {
        std::cerr << "Error: Failed to load model from " << filePath << std::endl;
        // Handle error appropriately, e.g., load a placeholder model or exit
    }
    return model;
}


void ResourceManager::LoadTerrainTexture() {
    terrainTexture = LoadTexture("assets/coast_sand_05_diff_4k.png");
    if (terrainTexture.id == 0) {
        std::cerr << "Error: Failed to load terrain texture." << std::endl;
    }
}

void ResourceManager::UnloadTerrainTexture() {
    UnloadTexture(terrainTexture);
}

Texture2D& ResourceManager::GetTerrainTexture() {
    return terrainTexture;
}

void ResourceManager::RemoveTerrainModel(const std::string& key) {
    if (terrainModels.count(key)) {
        UnloadModel(terrainModels[key]);
        terrainModels.erase(key);
    }
}


// Add, get, and remove terrain models
void ResourceManager::AddTerrainModel(const std::string& key, const Model& model) {
    terrainModels[key] = model;
}

Model& ResourceManager::GetTerrainModel(const std::string& key) {
    return terrainModels.at(key);
}


// Unload all terrain resources
void ResourceManager::UnloadAllTerrainResources() {
    for (auto& pair : terrainModels) {
        UnloadModel(pair.second);
    }
    terrainModels.clear();
}


void ResourceManager::UnloadResources() {
    // Unload models
    UnloadModel(playerModel);
    UnloadModel(enemyModel);
    UnloadModel(treeModel);
    UnloadModel(cloudModel);
    UnloadModel(missileModel);
    UnloadModel(bulletModel);
}
