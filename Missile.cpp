#include "Missile.h"
#include "GameSettings.h"
#include "GameUtilities.h"
#include "ResourceManager.h"
#include <iostream>
#include <cstdlib>


Missile::Missile(Vector3 startPosition, float scale)
{
}

void Missile::Update(float deltaTime)
{
   float smoothFactor = 0.02f * deltaTime;
   //position = Vector3MoveTowards(position, Vector3Lerp(position, (Vector3){position.x, position.y, 10.0f}, smoothFactor), 2);
}

void Missile::Draw()
{
    DrawModel(ResourceManager::GetMissileModel(),GetPosition(), GetScale(), WHITE);
}

