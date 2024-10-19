#include "Cloud.h"
#include "GameSettings.h"
#include "GameUtilities.h"
#include <iostream>
#include <cstdlib>
#include "ResourceManager.h"


Cloud::Cloud(Vector3 startPosition, float scale)
{
	SetPosition(startPosition);
    GeneratePosition();
	SetScale(scale);
}

void Cloud::GeneratePosition()
{
  	 float cloudXMin = -50.0f, cloudXMax = 50.0f;
     float cloudYMin = 15.0f, cloudYMax = 35.0f;

     Vector3 generatedPosition;
     generatedPosition.x = cloudXMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (cloudXMax - cloudXMin))); // Random x in range
     generatedPosition.y = cloudYMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (cloudYMax - cloudYMin)));
     generatedPosition.z = GetPosition().z - 100.0f; // Behind the player, modify as necessary
     SetScale(5.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 5.0f))); // Random size

	 SetPosition(generatedPosition);
}

void Cloud::Update(float deltaTime)
{
   float smoothFactor = 0.02f * deltaTime;

   SetPosition(Vector3MoveTowards(GetPosition(), Vector3Lerp(GetPosition(), (Vector3){GetPosition().x, GetPosition().y, 10.0f}, smoothFactor), 2));
}

void Cloud::Draw()
{
    DrawModel(ResourceManager::GetCloudModel(),GetPosition(),GetScale(), WHITE);
}
