#include "Cloud.h"
#include "GameSettings.h"
#include "GameUtilities.h"
#include <iostream>
#include <cstdlib>

Model Cloud::model;

Cloud::Cloud(Vector3 startPosition, float scale)
    : position(startPosition), scale(1.0f), active(true)
{
    position = GeneratePosition();
}

void Cloud::LoadModelFromResource()
{
    // Load the model
    std::string modelPath = "assets/Clouds/Cloud_Polygon_Blender_1.obj";
    model = LoadModel(modelPath.c_str());
    if (model.meshCount == 0)
    {
        std::cerr << "Error: Failed to load model from " << modelPath << std::endl;
    }
}
void Cloud::Unload()
{
    UnloadModel(model);
}

Vector3 Cloud::GeneratePosition()
{
  	 float cloudXMin = -50.0f, cloudXMax = 50.0f;
     float cloudYMin = 15.0f, cloudYMax = 35.0f;

      Vector3 generatedPosition;
     generatedPosition.x = cloudXMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (cloudXMax - cloudXMin))); // Random x in range
     generatedPosition.y = cloudYMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (cloudYMax - cloudYMin)));
     generatedPosition.z = position.z - 50.0f; // Behind the player, modify as necessary
     scale = 5.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 5.0f)); // Random size

     return generatedPosition;
}

void Cloud::Update(float deltaTime)
{
   float smoothFactor = 0.02f * deltaTime;

   position = Vector3MoveTowards(position, Vector3Lerp(position, (Vector3){position.x, position.y, 10.0f}, smoothFactor), 2);
}

void Cloud::Draw()
{
    if(active)
    {
        DrawModel(model, position, scale, WHITE);
    }
}

Vector3 Cloud::GetPosition() const
{
    return position;
}

bool Cloud::IsActive() const
{
    return active;
}

void Cloud::Deactivate()
{
    active = false;
}
