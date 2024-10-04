#include "Enemy.h"
#include "GameSettings.h"
#include "GameUtilities.h"
#include <iostream>


Enemy::Enemy(const std::string& modelPath, const std::string& texturePath,Vector3 startPosition, Vector3 size)
    : position(startPosition), size(size), active(true)
{

     // Load the model
    model = LoadModel(modelPath.c_str());
    if (model.meshCount == 0)
    {
        std::cerr << "Error: Failed to load model from " << modelPath << std::endl;
    }

    // Load the texture
    texture = LoadTexture(texturePath.c_str());
    if (texture.id == 0)
    {
        std::cerr << "Error: Failed to load texture from " << texturePath << std::endl;
    }

    // Apply the texture to the model
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

}

void Enemy::Unload()
{
    UnloadModel(model);
    UnloadTexture(texture);

}

void Enemy::Update(float deltaTime, const Vector3 targetPosition)
{
   float smoothFactor = 2.0f * deltaTime;
   position = Vector3MoveTowards(position, Vector3Lerp(position, targetPosition, smoothFactor), 2);
}

void Enemy::Draw()
{
    if(active)
    {
        DrawModel(model, position, 1.0f, WHITE);
    }
}

Vector3 Enemy::GetPosition() const
{
    return position;
}

bool Enemy::IsActive() const
{
    return active;
}

void Enemy::Deactivate()
{
    active = false;
}
