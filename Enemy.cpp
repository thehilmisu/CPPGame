#include "Enemy.h"

Enemy::Enemy(Vector3 startPosition, Vector3 size)
    : position(startPosition), size(size), active(true)
{
    // Assign a random color or use a predefined one
    color = RED;
}

Enemy::~Enemy()
{
    // Cleanup if necessary
}

void Enemy::Update(float deltaTime, Vector3 target)
{
   float smoothFactor = 1.0f * deltaTime;
   position = Vector3Lerp(position,Vector3Add(position, Vector3MoveTowards(position, target, 3)), smoothFactor);
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
