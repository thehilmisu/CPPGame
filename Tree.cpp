#include "Tree.h"
#include "GameSettings.h"
#include "GameUtilities.h"
#include "ResourceManager.h"
#include <iostream>
#include <cstdlib>

Tree::Tree(Vector3 startPosition, float scale)
    : position(startPosition), scale(1.0f), active(true)
{
    position = GeneratePosition();
}

Vector3 Tree::GeneratePosition()
{
  	 float cloudXMin = -50.0f, cloudXMax = 50.0f;
     float cloudYMin = 15.0f, cloudYMax = 35.0f;

     Vector3 generatedPosition;
     generatedPosition.x = cloudXMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (cloudXMax - cloudXMin))); // Random x in range
     generatedPosition.y = 0.0f;//cloudYMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (cloudYMax - cloudYMin)));
     generatedPosition.z = position.z - 50.0f; // Behind the player, modify as necessary
     scale = scale;//5.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 5.0f)); // Random size

     return generatedPosition;
}

void Tree::Update(float deltaTime)
{
   // we don't need to update the tree...
   //float smoothFactor = 0.02f * deltaTime;

   //position = Vector3MoveTowards(position, Vector3Lerp(position, (Vector3){position.x, position.y, 10.0f}, smoothFactor), 2);
}

void Tree::Draw()
{
    if(active)
    {
        DrawModel(ResourceManager::GetTreeModel(), position, scale, WHITE);
    }
}

Vector3 Tree::GetPosition() const
{
    return position;
}

bool Tree::IsActive() const
{
    return active;
}

void Tree::Deactivate()
{
    active = false;
}
