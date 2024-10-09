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

void ResourceManager::LoadResources() {
    // Load models
    playerModel = LoadModelFromFile(PLAYER_RAFALE_OBJ);
    enemyModel = LoadModelFromFile("assets/Enemy/enemy.obj");
    treeModel = LoadModelFromFile("assets/Tree/Tree.obj");
    cloudModel = LoadModelFromFile("assets/Clouds/Cloud_Polygon_Blender_1.obj");
    missileModel = LoadModelFromFile("assets/Missile/missile.obj");
    bulletModel = LoadModelFromFile("assets/Bullet/bullet.obj");
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
