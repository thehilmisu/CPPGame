#include "Tree.h"
#include "GameSettings.h"
#include "GameUtilities.h"
#include <iostream>
#include <cstdlib>


Tree::Tree(Vector3 startPosition, float scale)
    : position(startPosition), scale(1.0f), active(true)
{
     // Load the model
    std::string modelPath = "assets/Tree/Tree.obj";
    model = LoadModel(modelPath.c_str());
    if (model.meshCount == 0)
    {
        std::cerr << "Error: Failed to load model from " << modelPath << std::endl;
    }

    position = GeneratePosition();
}

void Tree::Unload()
{
    UnloadModel(model);
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
   float smoothFactor = 0.02f * deltaTime;

   //position = Vector3MoveTowards(position, Vector3Lerp(position, (Vector3){position.x, position.y, 10.0f}, smoothFactor), 2);
}

void Tree::Draw()
{
    if(active)
    {
        DrawModel(model, position, scale, WHITE);
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
