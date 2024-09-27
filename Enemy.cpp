#include "Enemy.h"

Enemy::Enemy(Vector3 startPosition, Vector3 size)
    : position(startPosition), size(size), active(true)
{
    color = RED;
}

Enemy::~Enemy()
{
    // Cleanup if necessary
}

void Enemy::Update(float deltaTime, Vector3 targetPosition)
{
   float smoothFactor = 2.0f * deltaTime;
   position = Vector3MoveTowards(position, Vector3Lerp(position, targetPosition, smoothFactor), 2);
}

void Enemy::Draw()
{
    if (active)
    {
        DrawCube(position, size.x, size.y, size.z, color);
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
