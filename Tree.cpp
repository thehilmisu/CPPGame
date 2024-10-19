#include "Tree.h"
#include "GameSettings.h"
#include "GameUtilities.h"
#include "ResourceManager.h"
#include <iostream>
#include <cstdlib>

Tree::Tree(Vector3 startPosition, float scale)
{
    SetPosition(startPosition);
	GeneratePosition();
    SetScale(scale);
}

void Tree::GeneratePosition()
{
  	 float cloudXMin = -50.0f, cloudXMax = 50.0f;
     float cloudYMin = 15.0f, cloudYMax = 35.0f;

     Vector3 generatedPosition;
     generatedPosition.x = cloudXMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (cloudXMax - cloudXMin))); // Random x in range
     generatedPosition.y = 0.0f;
     generatedPosition.z = GetPosition().z - 100.0f; // In front of the player

      SetPosition(generatedPosition);
}

void Tree::Update(float deltaTime)
{
	//GeneratePosition(playerPosition);
	//SetPosition(playerPosition);	
}

void Tree::Draw()
{
   DrawModel(ResourceManager::GetTreeModel(), GetPosition(), GetScale(), WHITE);
}
