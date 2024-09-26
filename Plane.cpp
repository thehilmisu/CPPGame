#include "Plane.h"
#include <iostream>


Plane::Plane(const std::string& modelPath, const std::string& texturePath, Vector3 startPosition)
    : position(startPosition), rotation(QuaternionIdentity()), scale({1.0f, 1.0f, 1.0f}),
      pitch(0.0f), yaw(0.0f), roll(0.0f), altitude(startPosition.y), speed(5.0f)
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

    // Initialize the transform
    UpdateTransform();
}

Plane::~Plane()
{
    UnloadModel(model);
    UnloadTexture(texture);
}


// Update method
void Plane::Update(float deltaTime)
{
    // Additional updates if necessary
    HandleInput(deltaTime);
}

// Draw method
void Plane::Draw()
{
    // Draw the model with its transform
    DrawModel(model, position, 1.0f, WHITE);
}

void Plane::SetPosition(const Vector3& newPosition)
{
    position = newPosition;
}

Vector3 Plane::GetPosition() const
{
    return position;
}

void Plane::SetRotation(const Quaternion& newRotation)
{
    rotation = newRotation;
}

Quaternion Plane::GetRotation() const
{
    return rotation;
}

void Plane::SetScale(const Vector3& newScale)
{
    scale = newScale;
}

Vector3 Plane::GetScale() const
{
    return scale;
}

void Plane::LookAt(const Vector3& targetPosition)
{
    // Calculate the direction vector from the player to the target in the XZ plane
    Vector3 direction = {
        targetPosition.x - position.x,
        0.0f, // Ignore Y-axis to prevent vertical rotation
        targetPosition.z - position.z
    };

    // Normalize the direction vector
    direction = Vector3Normalize(direction);

    // Calculate the yaw angle (rotation around Y-axis)
    float targetYaw = atan2f(direction.x, direction.z);

    // Convert yaw from radians to degrees
    float targetYawDegrees = RAD2DEG * targetYaw;

    // Set the player's rotation around the Y-axis
    rotation = QuaternionFromEuler(0.0f, targetYawDegrees * DEG2RAD, 0.0f);

    // Update the transform matrix
    UpdateTransform();
}


// Update rotation based on pitch, yaw, and roll
void Plane::UpdateRotation()
{
    // Convert pitch, yaw, and roll from degrees to radians
    float pitchRad = DEG2RAD * pitch;
    float yawRad = DEG2RAD * yaw;
    float rollRad = DEG2RAD * roll;

    // Create rotation quaternion from Euler angles
    Quaternion qPitch = QuaternionFromEuler(pitchRad, 0.0f, 0.0f);
    Quaternion qYaw = QuaternionFromEuler(0.0f, yawRad, 0.0f);
    Quaternion qRoll = QuaternionFromEuler(0.0f, 0.0f, rollRad);

    // Combine rotations: yaw * pitch * roll
    rotation = QuaternionMultiply(qYaw, QuaternionMultiply(qPitch, qRoll));

    // Update the transform matrix
    UpdateTransform();
}

void Plane::UpdateTransform()
{
    Matrix scaleMat = MatrixScale(scale.x, scale.y, scale.z);
    Matrix rotMat = QuaternionToMatrix(rotation);

    // Apply an additional rotation to flip the model
    Matrix adjustMat = MatrixRotateY(PI); // Rotate 180 degrees around Y-axis
    rotMat = MatrixMultiply(adjustMat, rotMat);

    Matrix transMat = MatrixTranslate(position.x, position.y, position.z);

    // Combine transformations: Translation * Rotation * Scale
    model.transform = MatrixMultiply(transMat, MatrixMultiply(rotMat, scaleMat));
}