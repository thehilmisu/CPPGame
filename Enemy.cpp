#include "Enemy.h"
#include "GameSettings.h"
#include "ResourceManager.h"
#include <iostream>


Enemy::Enemy(Vector3 startPosition, Vector3 size)
    : position(startPosition), size(size), active(true)
{


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
        DrawModel(ResourceManager::GetEnemyModel(), position, 1.0f, WHITE);
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
