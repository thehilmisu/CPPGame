#include "Plane.h"
#include <iostream>


Plane::Plane(const std::string& modelPath, const std::string& texturePath, Vector3 startPosition)
    : position(startPosition), rotation(QuaternionIdentity()), scale(1.0f),
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

void Plane::HandleInput(float deltaTime)
{
    // Adjust pitch (X-axis rotation)
    if (IsKeyDown(KEY_DOWN)) { pitch += 60.0f * deltaTime; altitude -= speed * deltaTime; }
    else if (IsKeyDown(KEY_UP)) { pitch -= 60.0f * deltaTime; altitude += speed * deltaTime; }
    else
    {
        if (pitch > 3.0f) pitch -= 30.0f * deltaTime;
        else if (pitch < -3.0f) pitch += 30.0f * deltaTime;
    }

    // Adjust yaw (Y-axis rotation)
    if (IsKeyDown(KEY_A)) yaw += 60.0f * deltaTime;
    else if (IsKeyDown(KEY_S)) yaw -= 60.0f * deltaTime;
    else
    {
        if (yaw > 0.0f) yaw -= 50.0f * deltaTime;
        else if (yaw < 0.0f) yaw += 50.0f * deltaTime;
    }

    // Adjust roll (Z-axis rotation) and horizontal movement
    float turningValue = 0.0f;
    if (IsKeyDown(KEY_RIGHT)) { roll -= 60.0f * deltaTime; turningValue += speed * deltaTime; }
    else if (IsKeyDown(KEY_LEFT)) { roll += 60.0f * deltaTime; turningValue -= speed * deltaTime; }
    else
    {
        if (roll > 6.0f * deltaTime) roll -= 60.0f * deltaTime;
        else if (roll < -6.0f * deltaTime) roll += 60.0f * deltaTime;
    }

    // Update position based on turning value
    position.x += turningValue;

    // Update altitude
    position.y = altitude;

    // Update the player's rotation based on pitch, yaw, and roll
    UpdateRotation();
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

void Plane::SetScale(const float& newScale)
{
    scale = newScale;
}

float Plane::GetScale() const
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

void Plane::Move(const Vector3& direction, float speed, float deltaTime)
{
    Vector3 movement = Vector3Scale(direction, speed * deltaTime);
    position = Vector3Add(position, movement);
    UpdateTransform();
}

void Plane::Rotate(const Vector3& axis, float angleDegrees)
{
    Quaternion q = QuaternionFromAxisAngle(axis, DEG2RAD * angleDegrees);
    rotation = QuaternionNormalize(QuaternionMultiply(q, rotation));
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
    Matrix scaleMat = MatrixScale(scale, scale, scale);
    Matrix rotMat = QuaternionToMatrix(rotation);

    // Apply an additional rotation to flip the model
    Matrix adjustMat = MatrixRotateY(PI); // Rotate 180 degrees around Y-axis
    rotMat = MatrixMultiply(adjustMat, rotMat);

    Matrix transMat = MatrixTranslate(position.x, position.y, position.z);

    // Combine transformations: Translation * Rotation * Scale
    model.transform = MatrixMultiply(transMat, MatrixMultiply(rotMat, scaleMat));
}